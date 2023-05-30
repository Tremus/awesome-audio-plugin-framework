cmake_minimum_required(VERSION 3.15)

add_executable(open_file_dialog
    open_file_dialog.c
)

target_link_libraries(open_file_dialog
    tinyfiledialogs
)

target_include_directories(open_file_dialog PUBLIC
    ../../modules/tinyfiledialogs
)
