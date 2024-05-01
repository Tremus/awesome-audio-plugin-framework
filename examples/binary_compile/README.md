# Binary Compile

This example shows you how to take any file and turn it into a big array of bytes in a `.c` file, which can then be accessible in your programs by using a simple `const unsigned char*`.

A real world example of when you might need a tool like this is if you're building a GUI application that depends on particular fonts and images. You may prefer to simply include the file data within your executable or library and access the data through a pointer, rather than depend on the existance of the files a particular location and loading them at runtime.

This folder contains 2 projects:
1. [binary_serialise](serialise.c) - A simple command line tool that takes an input filepath, and outputs files binary to `{filename}_bin.h` & `{filename}_bin.c`. The CMake project is configured to make it serialise [assets/myfile.txt](assets/myfile.txt)
2. [binary_compile](main.c) - Prints to the console the data within `assets/myfile_bin.c`.

Using some very simple CMake tricks, if you try to run binary_compile, CMake will automatically detect file changes to `assets/myfile.txt`, build/rebuild `binary_serialise` if necessary, and serialise/reserialise `myfile.txt` if necessary.

## Alternatives

This is akin to other tools such as [juceaid](https://github.com/juce-framework/JUCE) and [bin2c](https://github.com/adobe/bin2c).

My gripe with juceaid is that it uses the JUCE framework as a dependancy. This dependancy is neither small, nor is the licensing permissive. juceaid takes an unacceptable amount of time to build and run in an automated build process.

bin2c is small, permissively licensed, easy to use, and fast to build & run. As a nice to have, the compiled `.c` files are about 25% smaller than what this program will generate. This is achieved by serialising the data into a C string. Unfortunately the dealbreaker for me is that MSVC sometimes screams at you with errors if you try to compile the giant generated string in the `.c` file. Clang doesn't have this problem.

## Why use this?

- Do your build scripts run through CMake?
- Are your assets are fairly small (<10mb) / you don't care about generated `.c` file size?
- Do you like SPEED and FREE?

If you answered yes to all of the above then perhaps some ideas from this example can help you. Feel free to take what you like.