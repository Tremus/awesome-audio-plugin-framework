cmake_minimum_required(VERSION 3.15)
set(CMAKE_C_STANDARD 99)

add_executable(image_stack_blur main.c)

target_compile_definitions(image_stack_blur PRIVATE
    -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(image_stack_blur PRIVATE ../../modules/stb)