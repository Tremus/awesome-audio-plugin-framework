#!/bin/sh
rm -r build
mkdir -p build
clang -g -framework CoreFoundation make_stacktrace.c -o ./build/make_stacktrace

./build/make_stacktrace

objdump -t ./build/make_stacktrace

# https://developer.apple.com/documentation/xcode/adding-identifiable-symbol-names-to-a-crash-report

xcrun dsymutil "./build/make_stacktrace" -o "./build/make_stacktrace.dSYM"
strip "./build/make_stacktrace"

objdump -t ./build/make_stacktrace
