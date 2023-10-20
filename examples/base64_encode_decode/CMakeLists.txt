cmake_minimum_required(VERSION 3.15)
project(base64_encode_decode VERSION 0.0.1 LANGUAGES C)

add_executable(base64_encode_decode main.c)

target_include_directories(base64_encode_decode PRIVATE
    ../../modules/base64/include
    ../../modules/base64/lib
    )
target_compile_options(base64_encode_decode PRIVATE "-mssse3" "-mavx2" "-mavx512f" "-mavx512vbmi" "-mfpu=neon")