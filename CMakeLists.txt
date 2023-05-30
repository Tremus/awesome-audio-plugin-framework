cmake_minimum_required(VERSION 3.15)
project(AAPF)

#### BUILD LIBRARIES ####

add_library(tinyfiledialogs STATIC modules/tinyfiledialogs/tinyfiledialogs.c)
set(TINYCTHREAD_DISABLE_TESTS FALSE)
add_subdirectory(modules/tinycthread)


#### BUILD EXAMPLES ####

add_subdirectory(examples/open_file_dialog)
