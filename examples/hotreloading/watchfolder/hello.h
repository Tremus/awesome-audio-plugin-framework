#pragma once
typedef void (*hello_t)();
#ifdef _WIN32
__declspec(dllexport) void hello();
#else
__attribute__((visibility("default"))) void hello();
#endif