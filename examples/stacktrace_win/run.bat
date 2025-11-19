echo "Hello"

RMDIR build /S /Q
MKDIR build

clang -g main.c -o build\make_stacktrace.exe

.\build\make_stacktrace