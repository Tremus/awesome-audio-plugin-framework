#include "watchfolder/hello.h"
#include <signal.h>
#include <stdio.h>
#include <windows.h>

#define assert(cond) (cond) ? (void)0 : __debugbreak()

// Resources:
// https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
// https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibrarya
// https://learn.microsoft.com/en-us/windows/win32/procthread/creating-processes
// https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-readdirectorychangesw

DWORD         WatchFileChangesProc(void* ptr);
volatile LONG flag_should_rebuild         = 0;
int           flag_exit_file_watch_thread = 0;

// Get time func taken from here https://gist.github.com/jspohr/3dc4f00033d79ec5bdaf67bc46c813e3
LARGE_INTEGER       timer_freq, timer_start;
static inline INT64 get_now_ns()
{
    LARGE_INTEGER perf_now;
    QueryPerformanceCounter(&perf_now);
    perf_now.QuadPart -= timer_start.QuadPart;
    INT64 q           = perf_now.QuadPart / timer_freq.QuadPart;
    INT64 r           = perf_now.QuadPart % timer_freq.QuadPart;
    return q * 1000000000 + r * 1000000000 / timer_freq.QuadPart;
}

int  flag_exit_main_func = 0;
void ctrlc_cb(int ignore) { flag_exit_main_func = 1; }

int main()
{
    QueryPerformanceFrequency(&timer_freq);
    QueryPerformanceCounter(&timer_start);

    HANDLE hWatchThread = CreateThread(NULL, 0, &WatchFileChangesProc, NULL, 0, 0);

    HMODULE lib = LoadLibrary(HOTRELOAD_LIB_PATH);
    assert(lib != NULL);
    hello_t libfunc = (hello_t)GetProcAddress(lib, "hello");
    assert(libfunc != NULL);

    signal(SIGINT, ctrlc_cb);
    while (flag_exit_main_func == 0)
    {
        LONG rebuild = InterlockedExchange(&flag_should_rebuild, 0);

        if (rebuild)
        {
            UINT64 reload_start_ns = get_now_ns();
            FreeLibrary(lib);

            UINT64 build_start_ns = get_now_ns();

            // Using 'system' from lib C is way simpler, but creates some stdout buffering problems...
            // Instead, we do it the windows way
            // The idea is we create a new process using CREATE_NEW_CONSOLE while setting HIDE falgs in the STARTUPINFO
            STARTUPINFO         si;
            PROCESS_INFORMATION pi;

            ZeroMemory(&si, sizeof(si));
            si.cb      = sizeof(si);
            si.dwFlags = STARTF_USESHOWWINDOW; // These flags here are necessarry to stop a terminal window popping up
            si.wShowWindow = SW_HIDE;          // during the process
            ZeroMemory(&pi, sizeof(pi));

            // Start the child process.
            if (! CreateProcess(0, HOTRELOAD_BUILD_COMMAND, 0, 0, FALSE, CREATE_NEW_CONSOLE, 0, 0, &si, &pi))
            {
                printf("CreateProcess failed (%lu).\n", GetLastError());
                return 1;
            }

            // Wait until child process exits.
            WaitForSingleObject(pi.hProcess, INFINITE);

            // Close process and thread handles.
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            UINT64 build_end_ns = get_now_ns();

            lib = LoadLibrary(HOTRELOAD_LIB_PATH);
            assert(lib != NULL);
            libfunc = (hello_t)GetProcAddress(lib, "hello");
            assert(libfunc != NULL);
            UINT64 reload_end_ns = get_now_ns();

            double rebuild_ms = (double)(build_end_ns - build_start_ns) / 1000000.0;
            double reload_ms  = (double)(reload_end_ns - reload_start_ns) / 1000000.0;
            fprintf(stderr, "Rebuild time %.2fms\n", rebuild_ms);
            fprintf(stderr, "Reload time %.2fms\n", reload_ms);
        }

        libfunc();
        Sleep(100);
    }

    flag_exit_file_watch_thread = 1;

    if (hWatchThread)
    {
        WaitForSingleObject(hWatchThread, INFINITE);
        CloseHandle(hWatchThread);
    }
    return 0;
}

DWORD WatchFileChangesProc(void* ptr)
{
    // Most this code was taken from here: https://gist.github.com/nickav/a57009d4fcc3b527ed0f5c9cf30618f8
    fprintf(stderr, "Watching folder %s\n", HOTRELOAD_WATCH_DIR);

    HANDLE hDirectory = CreateFile(
        HOTRELOAD_WATCH_DIR,
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        NULL);
    if (hDirectory == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Failed to get directory handle\n");
        return 1;
    }
    OVERLAPPED overlapped;
    overlapped.hEvent = CreateEvent(NULL, FALSE, 0, NULL);

    BYTE infobuffer[1024];
    BOOL success = ReadDirectoryChangesW(
        hDirectory,
        infobuffer,
        sizeof(infobuffer),
        TRUE,
        FILE_NOTIFY_CHANGE_LAST_WRITE,
        NULL,
        &overlapped,
        NULL);
    if (! success)
    {
        fprintf(stderr, "Failed to queue info buffer\n");
        return 1;
    }

    while (flag_exit_file_watch_thread == 0)
    {
        DWORD result = WaitForSingleObject(overlapped.hEvent, 50);

        if (result == WAIT_OBJECT_0)
        {
            DWORD bytes_transferred;
            GetOverlappedResult(hDirectory, &overlapped, &bytes_transferred, FALSE);

            FILE_NOTIFY_INFORMATION* event = (FILE_NOTIFY_INFORMATION*)infobuffer;

            while (TRUE)
            {
                DWORD name_len = event->FileNameLength / sizeof(wchar_t);

                if (event->Action == FILE_ACTION_MODIFIED)
                {
                    fwprintf(stderr, L"Changed: %.*s\n", name_len, event->FileName);
                    InterlockedExchange(&flag_should_rebuild, 1);
                }

                // Are there more events to handle?
                if (event->NextEntryOffset)
                    *((BYTE**)&event) += event->NextEntryOffset;
                else
                    break;
            }

            // Queue the next event
            success = ReadDirectoryChangesW(
                hDirectory,
                infobuffer,
                sizeof(infobuffer),
                TRUE,
                FILE_NOTIFY_CHANGE_LAST_WRITE,
                NULL,
                &overlapped,
                NULL);

            if (! success)
            {
                fprintf(stderr, "Failed to queue info buffer\n");
                return 1;
            }
        }
    }
    return 0;
}