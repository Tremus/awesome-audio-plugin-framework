cmake_minimum_required(VERSION 3.15)

add_executable(base64_encode_decode main.c)

target_include_directories(base64_encode_decode PRIVATE ../../modules/base64/include)

if(WIN32)
    target_compile_options(base64_encode_decode PRIVATE "-mssse3" "-mavx2" "-mavx512f" "-mavx512vbmi")
endif()