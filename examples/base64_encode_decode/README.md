# Base64

Simple base64 string encoding & decoding.

Apparently [this repo](https://github.com/powturbo/Turbo-Base64) has the fastest encoder + decoder, but it's GPL3 licensed...
In this example we use the [Tremus fork](https://github.com/Tremus/base64) of [this library](https://github.com/aklomp/base64) (BSD2 license). The original library has a more complicated CMakeLists.txt file which slows down builds and it heavily relies on preprocessor macros. The Tremus fork cleans most of that up and creates a neat single compile unit.
