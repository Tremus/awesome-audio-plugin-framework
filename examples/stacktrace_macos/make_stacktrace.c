#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlfcn.h>
#include <execinfo.h>
#include <unistd.h>

#include <CoreFoundation/CoreFoundation.h>
#include <mach-o/dyld.h>

void errlog(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

void print_stack_trace()
{
    // https://forum.juce.com/t/finding-the-binary-image-address-range-in-macos/21539
    // https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/dladdr.3.html
    // https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/backtrace.3.html
    Dl_info info           = {0};
    void*   callstack[128] = {0};
    int     i = 0, frames = 0;
    char**  strs   = 0;
    FILE*   stream = 0;

    static char errlog_path[1024] = {0};

    int ok = dladdr(print_stack_trace, &info);

    if (errlog_path[0] == 0)
    {
        getcwd(errlog_path, sizeof(errlog_path));
        if (errlog_path[0] != 0)
        {
            int      pathlen = strlen(errlog_path);
            uint64_t now     = CFAbsoluteTimeGetCurrent() + kCFAbsoluteTimeIntervalSince1970;
            snprintf(errlog_path + pathlen, sizeof(errlog_path) - pathlen, "/stacktrace_%llu.txt", now);
            printf("%s\n", errlog_path);
        }
    }

    if (errlog_path[0] != 0)
        stream = freopen(errlog_path, "a", stderr);

    if (ok == 0)
    {
        perror("dladdr()");
    }
    else
    {
        errlog("Binary Path: %s\n", info.dli_fname);
        errlog("Base Address: %p\n", info.dli_fbase);
    }

    frames = backtrace(callstack, 128);
    strs   = backtrace_symbols(callstack, frames);

    if (strs == NULL)
        perror("backtrace_symbols()");

    for (i = 0; i < frames; ++i)
    {
        errlog("%s\n", strs[i]);
    }

    if (strs)
        free(strs);
    if (stream)
        fclose(stream);
}

void function_c() { print_stack_trace(); }

void function_b() { function_c(); }

void function_a() { function_b(); }

int main(int argc, char** argv)
{
    function_a();

    return 0;
}
