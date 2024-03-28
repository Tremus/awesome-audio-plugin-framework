cmake_minimum_required(VERSION 3.5)
project(file_searching)

add_executable(${PROJECT_NAME} main.c)
target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_DIR="${CMAKE_BINARY_DIR}")