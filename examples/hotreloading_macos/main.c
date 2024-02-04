#include "watchfolder/hello.h"
#include <CoreServices/CoreServices.h>
#include <dlfcn.h>

#include <mach/mach_time.h>

#define SLEEP_MS(ms) usleep(ms * 1000)
#define ARRSIZE(a) (sizeof(a) / sizeof(a[0]))

void*   lib     = NULL;
hello_t libfunc = NULL;
int     load_libs();
void    unload_libs();

mach_timebase_info_data_t g_timebase;
uint64_t                  g_machtime_start = 0;

volatile int flag_file_changed_in_watchdir = 0;
void         fsevent_cb(
            ConstFSEventStreamRef,
            void*,
            size_t,
            void*,
            const FSEventStreamEventFlags*,
            const FSEventStreamEventId*);

int  flag_exit_event_loop = 0;
void ctrlc_cb(int ignore) { flag_exit_event_loop = 1; }

static inline uint64_t get_now_ns()
{
    const uint64_t mach_now = mach_absolute_time() - g_machtime_start;
    return (mach_now / g_timebase.denom) * g_timebase.numer +
           (mach_now % g_timebase.denom) * g_timebase.numer / g_timebase.denom;
}

int main()
{
    int code = load_libs();
    if (code != 0)
        return code;

    mach_timebase_info(&g_timebase);
    g_machtime_start = mach_absolute_time();

    // https://developer.apple.com/documentation/coreservices/file_system_events?language=objc
    static const void* watchpaths[]  = {CFSTR(HOTRELOAD_WATCH_DIR)};
    CFArrayRef         arrref        = CFArrayCreate(NULL, (const void**)&watchpaths, ARRSIZE(watchpaths), NULL);
    double             onehundred_ms = 0.1;
    FSEventStreamRef   stream        = FSEventStreamCreate(
        NULL,
        &fsevent_cb,
        NULL,
        arrref,
        kFSEventStreamEventIdSinceNow,
        onehundred_ms,
        kFSEventStreamCreateFlagUseCFTypes | kFSEventStreamCreateFlagFileEvents);

    // https://developer.apple.com/documentation/dispatch/dispatch_queue
    dispatch_queue_t queue = dispatch_queue_create("File & Library watcher", DISPATCH_QUEUE_SERIAL);
    assert(queue != NULL);

    FSEventStreamSetDispatchQueue(stream, queue);
    bool started = FSEventStreamStart(stream);
    assert(started);

    // Set up callback for user hitting Ctrl-C
    signal(SIGINT, ctrlc_cb);
    while (flag_exit_event_loop == 0)
    {
        int change = __atomic_exchange_n(&flag_file_changed_in_watchdir, 0, __ATOMIC_SEQ_CST);
        if (change)
        {
            UInt64 reload_start_ns = get_now_ns();
            unload_libs();

            UInt64 build_start_ns = get_now_ns();
            code                  = system(HOTRELOAD_BUILD_COMMAND);
            UInt64 build_end_ns   = get_now_ns();

            if (code != 0)
            {
                fprintf(stderr, "ERROR: Build exited with code %d\n", code);
                return code;
            }

            code = load_libs();
            if (code != 0)
                return code;
            UInt64 reload_end_ns = get_now_ns();

            double rebuild_ms = (double)(build_end_ns - build_start_ns) / 1000000.0;
            double reload_ms  = (double)(reload_end_ns - reload_start_ns) / 1000000.0;
            printf("Rebuild time %.2fms\n", rebuild_ms);
            printf("Reload time %.2fms\n", reload_ms);
        }

        libfunc();
        SLEEP_MS(1000);
    }

    printf("Shutting down\n");
    unload_libs();
    FSEventStreamStop(stream);
    FSEventStreamSetDispatchQueue(stream, NULL);
    FSEventStreamInvalidate(stream);
    FSEventStreamRelease(stream);
    dispatch_release(queue);
    return 0;
}

#pragma region LIB_LOADING

int load_libs()
{
    assert(lib == NULL);
    lib = dlopen(HOTRELOAD_LIB_PATH, RTLD_NOW);
    if (lib == NULL)
    {
        printf("Failed opening library at path: " HOTRELOAD_LIB_PATH);
        return 1;
    }

    assert(libfunc == NULL);
    libfunc = (hello_t)dlsym(lib, "hello");
    if (libfunc == NULL)
    {
        printf("Failed finding library function \"hello()\"");
        return 1;
    }
    return 0;
}
void unload_libs()
{
    if (lib != NULL)
        dlclose(lib);
    lib     = NULL;
    libfunc = NULL;
}

#pragma endregion LIB_LOADING

#pragma region FILEWATCHING
// Saving a file ususually also contains the flag kFSEventStreamEventFlagItemInodeMetaMod
static const FSEventStreamEventFlags kFSEventStreamEventFlagFileModified =
    kFSEventStreamEventFlagItemModified | kFSEventStreamEventFlagItemIsFile;

void fsevent_cb(
    ConstFSEventStreamRef          streamRef,
    void*                          clientCallBackInfo,
    size_t                         numEvents,
    void*                          eventPaths,
    const FSEventStreamEventFlags* eventFlags,
    const FSEventStreamEventId*    eventIds)
{
    for (int i = 0; i < numEvents; i++)
    {
        CFArrayRef  arr  = (CFArrayRef)eventPaths;
        CFStringRef val  = CFArrayGetValueAtIndex(arr, i);
        const char* path = CFStringGetCStringPtr(val, 0);

        if ((eventFlags[i] & kFSEventStreamEventFlagFileModified) == kFSEventStreamEventFlagFileModified)
        {
            fprintf(stderr, "File change: %s\n", path);
            __atomic_store_n(&flag_file_changed_in_watchdir, 1, __ATOMIC_SEQ_CST);
        }
    }
}
#pragma endregion FILEWATCHING