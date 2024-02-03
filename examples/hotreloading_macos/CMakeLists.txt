cmake_minimum_required(VERSION 3.15)
project(hotreload_macos VERSION 0.0.1 LANGUAGES C)

set(HOTRELOAD_LIB_NAME hotreloadable_lib_macos)

add_library(${HOTRELOAD_LIB_NAME} MODULE EXCLUDE_FROM_ALL watchfolder/hello.c)

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE "-framework CoreServices")

# cmake_path(
#         RELATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}
#         BASE_DIRECTORY ${CMAKE_SOURCE_DIR}
#         OUTPUT_VARIABLE project_rel_path)
file(RELATIVE_PATH project_rel_path ${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR})
message(STATUS "hello ${project_rel_path}")

target_compile_definitions(${PROJECT_NAME} PRIVATE
    -DHOTRELOAD_WATCH_DIR="${PROJECT_SOURCE_DIR}/watchfolder"
    -DHOTRELOAD_LIB_PATH="${CMAKE_BINARY_DIR}/${project_rel_path}/lib${HOTRELOAD_LIB_NAME}.so"
    -DHOTRELOAD_BUILD_COMMAND="cmake --build ${CMAKE_BINARY_DIR} --config Debug --target ${HOTRELOAD_LIB_NAME}"
    )

add_dependencies(${PROJECT_NAME} ${HOTRELOAD_LIB_NAME})