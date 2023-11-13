# Stack Blur

Demonstrates the use of the Stack Blur algorithm, a CPU side algorithm that looks similar to the Gaussian blur while being reasonably fast.

Additionally it showcases use of the STB image & image write libraries for reading & writing of images files

It should be noted that the blur algorithm only accepts 8bit colour channels in RGBA format. The fourth alpha channel is mandatory in this version of the algorithm.

The STB library functions in this example are called to read & write in this RGBA format. They do support plain 8bit RGB if you need that format elsewhere...
