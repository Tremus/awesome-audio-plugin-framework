cmake_minimum_required(VERSION 3.15)
project(AAPF)

if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wno-deprecated)
endif()
#### BUILD EXAMPLES ####

add_subdirectory(examples/base64_encode_decode)
add_subdirectory(examples/fft)
add_subdirectory(examples/file_searching)
add_subdirectory(examples/hotreloading)
add_subdirectory(examples/https_curl)
add_subdirectory(examples/https_cute_tls)
add_subdirectory(examples/image_stack_blur)
add_subdirectory(examples/binary_compile)
add_subdirectory(examples/osdialog_open_save_file)
add_subdirectory(examples/tinyfiledialogs_open_file)
add_subdirectory(examples/tinyfiledialogs_save_file)
add_subdirectory(examples/tweetnacl)

add_subdirectory(templates/cplug_pugl_nanovg)