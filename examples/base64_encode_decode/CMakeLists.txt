cmake_minimum_required(VERSION 3.15)

add_executable(base64_encode_decode main.c)

target_include_directories(base64_encode_decode PRIVATE ../../modules/base64/include)
target_compile_options(base64_encode_decode PRIVATE "-mssse3" "-mavx2" "-mavx512f" "-mavx512vbmi" "-mfpu=neon")