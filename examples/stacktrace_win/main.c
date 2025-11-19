#define _CRT_SECURE_NO_WARNINGS
#define UNICODE

#include <windows.h>

#include <Psapi.h>

#include <dbghelp.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "DbgHelp.lib")

static LONG WINAPI MyHandler(EXCEPTION_POINTERS* pExceptionInfo)
{
    const LPVOID ExceptionAddress = pExceptionInfo->ExceptionRecord->ExceptionAddress;
    const DWORD  ExceptionCode    = pExceptionInfo->ExceptionRecord->ExceptionCode;

    static WCHAR logpath[MAX_PATH] = {0};
    FILE*        stream            = 0;

    if (logpath[0] == 0)
    {
        _wgetcwd(logpath, MAX_PATH);
        if (logpath[0] != 0)
        {
            size_t len = wcslen(logpath);
            _snwprintf(logpath + len, ARRAYSIZE(logpath) - len, L"\\stacktrace.txt");

            // OutputDebugStringW(logpath);
            // OutputDebugStringW(L"\n");
        }
    }

    if (logpath[0] != 0)
        stream = _wfreopen(logpath, L"a", stderr);

    HANDLE hProcess = GetCurrentProcess();

    static int sym_initialized = 0;
    if (!sym_initialized)
    {
        SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME);
        if (!SymInitialize(hProcess, NULL, TRUE))
        {
            fprintf(stderr, "Failed to initialize symbols.\n");
            return EXCEPTION_EXECUTE_HANDLER;
        }
        sym_initialized = 1;
    }

    // https://learn.microsoft.com/en-us/windows/win32/api/winnt/nf-winnt-rtlcapturestackbacktrace
    void*      StackFrames[64];
    const WORD NumFrames = RtlCaptureStackBackTrace(0, ARRAYSIZE(StackFrames), StackFrames, NULL);

    // https://learn.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-getprocessimagefilenamew
    static WCHAR ProcessFileName[MAX_PATH] = {0};
    DWORD        ProcessFileNameLen = GetProcessImageFileNameW(hProcess, ProcessFileName, ARRAYSIZE(ProcessFileName));

    if (ProcessFileNameLen)
    {
        fprintf(stderr, "Process file path: %.*ls\n", (int)ProcessFileNameLen, ProcessFileName);
    }

    // https://stackoverflow.com/questions/14467229/get-base-address-of-process
    static HMODULE    Modules[256];
    static MODULEINFO ModulesInfo[ARRAYSIZE(Modules)];
    static WCHAR      ModuleName[MAX_PATH];
    DWORD             ModuleNameLen        = 0;
    DWORD             RequiredBytesModules = 0;
    HMODULE           CrashedModule        = NULL;
    LPMODULEINFO      CrashedModuleInfo    = NULL;

    // https://learn.microsoft.com/en-au/windows/win32/api/psapi/nf-psapi-enumprocessmodules
    BOOL haveModules = EnumProcessModules(hProcess, Modules, sizeof(Modules), &RequiredBytesModules);
    if (haveModules)
    {
        for (int i = 0; i < RequiredBytesModules / sizeof(HMODULE); i++)
        {
            HMODULE hModule = Modules[i];
            // https://learn.microsoft.com/en-au/windows/win32/api/psapi/nf-psapi-getmoduleinformation
            LPMODULEINFO lpInfo         = &ModulesInfo[i];
            BOOL         haveModuleInfo = GetModuleInformation(hProcess, hModule, lpInfo, sizeof(*lpInfo));
            if (haveModuleInfo)
            {
                LPVOID StartAddress = lpInfo->lpBaseOfDll;
                LPVOID EndAddress   = lpInfo->lpBaseOfDll + lpInfo->SizeOfImage;
                if (ExceptionAddress >= StartAddress && ExceptionAddress < EndAddress)
                {
                    CrashedModule     = Modules[i];
                    CrashedModuleInfo = &ModulesInfo[i];
                    // https://learn.microsoft.com/en-au/windows/win32/api/psapi/nf-psapi-getmodulefilenameexa
                    ModuleNameLen = GetModuleFileNameExW(hProcess, hModule, ModuleName, ARRAYSIZE(ModuleName));
                }

                // if (ModuleNameLen)
                // {
                //     fprintf(
                //         stderr,
                //         "Loaded Module: 0x%p:0x%p - %.*ls\n",
                //         StartAddress,
                //         EndAddress,
                //         (int)ModuleNameLen,
                //         ModuleName);
                // }
            }
        }
    }
    if (CrashedModuleInfo && ModuleNameLen)
    {
        fprintf(
            stderr,
            "Crashed Module: 0x%p:0x%p - %.*ls\n",
            CrashedModuleInfo->lpBaseOfDll,
            CrashedModuleInfo->lpBaseOfDll + CrashedModuleInfo->SizeOfImage,
            (int)ModuleNameLen,
            ModuleName);
    }
    fprintf(stderr, "ExceptionCode: %lu\n", ExceptionCode);
    fprintf(stderr, "ExceptionAddress: 0x%p\n", ExceptionAddress);

    static SYMBOL_INFO* pSymbol                                 = NULL;
    static BYTE         Symbolbuffer[sizeof(SYMBOL_INFO) + 256] = {0};
    pSymbol                                                     = (SYMBOL_INFO*)Symbolbuffer;
    pSymbol->SizeOfStruct                                       = sizeof(SYMBOL_INFO);
    pSymbol->MaxNameLen                                         = 255;

    for (USHORT i = 0; i < NumFrames; ++i)
    {
        DWORD64 addr = (DWORD64)(ULONG_PTR)StackFrames[i];

        IMAGEHLP_LINE64 line;

        DWORD64 SymbolDisplacement = 0;
        DWORD   LineDisplacement   = 0;

        // https://learn.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-symfromaddr
        BOOL haveSymbol = SymFromAddr(hProcess, addr, &SymbolDisplacement, pSymbol);

        // https://learn.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-symgetlinefromaddr64
        BOOL haveLine = SymGetLineFromAddr64(hProcess, addr, &LineDisplacement, &line);

        fprintf(stderr, "%2u: ", i);
        if (haveSymbol)
        {
            fprintf(stderr, "%s+0x%llX", pSymbol->Name, SymbolDisplacement);
        }
        else
        {
            fprintf(stderr, "<unknown>");
        }

        if (haveLine)
        {
            fprintf(stderr, " - %s:%lu +0x%lX\n", line.FileName, line.LineNumber, LineDisplacement);
        }
        else
        {
            fprintf(stderr, "\n");
        }
    }
    fprintf(stderr, "\n");

    if (stream)
        fclose(stream);

    return EXCEPTION_EXECUTE_HANDLER;
}

void Level3(void);
void Level2(void) { Level3(); }
void Level1(void) { Level2(); }

void Level3(void)
{
    // do segfault
    int* my_nullptr = NULL;
    *my_nullptr     = 69;
}

int main(void)
{
    // https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-addvectoredexceptionhandler
    HANDLE hVeth = AddVectoredExceptionHandler(1, MyHandler);
    if (!hVeth)
    {
        fprintf(stderr, "AddVectoredExceptionHandler failed.\n");
        return 1;
    }

    Level1();

    // https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-removevectoredexceptionhandler
    RemoveVectoredExceptionHandler(hVeth);
    return 0;
}
