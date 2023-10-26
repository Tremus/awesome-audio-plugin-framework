cmake_minimum_required(VERSION 3.15)
add_executable(fft main.c)
target_include_directories(fft PRIVATE ../../modules/pffft)