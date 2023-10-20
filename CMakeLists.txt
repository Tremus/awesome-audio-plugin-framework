cmake_minimum_required(VERSION 3.15)
project(AAPF)

#### BUILD LIBRARIES ####

add_library(tinyfiledialogs STATIC modules/tinyfiledialogs/tinyfiledialogs.c)
set(TINYCTHREAD_DISABLE_TESTS FALSE)
set(TINYCTHREAD_DISABLE_TESTS ON)
add_subdirectory(modules/tinycthread)

#### BUILD EXAMPLES ####

add_subdirectory(examples/base64_encode_decode)
add_subdirectory(examples/https_curl)
add_subdirectory(examples/https_cute_tls)
add_subdirectory(examples/open_file_dialog)
add_subdirectory(examples/save_file_dialog)
add_subdirectory(examples/tweetnacl)