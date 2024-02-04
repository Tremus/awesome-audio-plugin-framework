cmake_minimum_required(VERSION 3.15)
project(hotreloading VERSION 0.0.1 LANGUAGES C)

set(HOTRELOAD_LIB_NAME hotreloadable_lib)

add_library(${HOTRELOAD_LIB_NAME} MODULE watchfolder/hello.c)

file(RELATIVE_PATH project_rel_path ${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR})

if (WIN32)
    add_executable(${PROJECT_NAME} main_windows.c)
    
    # Windows paths are complicated
    set(HOTRELOAD_WATCH_DIR_POSIX "${PROJECT_SOURCE_DIR}/watchfolder")
    set(HOTRELOAD_LIB_PATH_POSIX "${CMAKE_BINARY_DIR}/${project_rel_path}/${HOTRELOAD_LIB_NAME}.dll")
    string(REPLACE "/" "\\\\" HOTRELOAD_WATCH_DIR "${HOTRELOAD_WATCH_DIR_POSIX}")
    string(REPLACE "/" "\\\\" HOTRELOAD_LIB_PATH "${HOTRELOAD_LIB_PATH_POSIX}")
    
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        -DHOTRELOAD_WATCH_DIR="${HOTRELOAD_WATCH_DIR}"
        -DHOTRELOAD_LIB_PATH="${HOTRELOAD_LIB_PATH}"
        -DHOTRELOAD_BUILD_COMMAND="cmake --build ${CMAKE_BINARY_DIR} --config Debug --target ${HOTRELOAD_LIB_NAME}"
        )

else()
    add_executable(${PROJECT_NAME} main_macos.c)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework CoreServices")
    
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        -DHOTRELOAD_WATCH_DIR="${PROJECT_SOURCE_DIR}/watchfolder"
        -DHOTRELOAD_LIB_PATH="${CMAKE_BINARY_DIR}/${project_rel_path}/lib${HOTRELOAD_LIB_NAME}.so"
        -DHOTRELOAD_BUILD_COMMAND="cmake --build ${CMAKE_BINARY_DIR} --config Debug --target ${HOTRELOAD_LIB_NAME}"
        )
endif()

add_dependencies(${PROJECT_NAME} ${HOTRELOAD_LIB_NAME})