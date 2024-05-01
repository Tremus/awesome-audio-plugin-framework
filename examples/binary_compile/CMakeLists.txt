cmake_minimum_required(VERSION 3.15)
project(binary_compile VERSION 0.0.1 LANGUAGES C)

add_executable(binary_serialise serialise.c)
add_executable(binary_compile main.c ${PROJECT_SOURCE_DIR}/assets/myfile_bin.c)

# binary_serialise will always be built before building/running binary_compile
add_dependencies(binary_compile binary_serialise)

# This detects changes to myfile.txt, re-runs binary_serialise, writing binary data into myfile_bin.c
file(RELATIVE_PATH project_rel_path ${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR})
add_custom_command(
    OUTPUT ${PROJECT_SOURCE_DIR}/assets/myfile_bin.c
    COMMAND echo "Recompiling assets/myfile.txt"
    COMMAND ${CMAKE_BINARY_DIR}/${project_rel_path}/binary_serialise ${PROJECT_SOURCE_DIR}/assets/myfile.txt
    DEPENDS ${PROJECT_SOURCE_DIR}/assets/myfile.txt)