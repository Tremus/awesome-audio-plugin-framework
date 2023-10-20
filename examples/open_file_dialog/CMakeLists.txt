cmake_minimum_required(VERSION 3.15)

add_executable(open_file_dialog open_file_dialog.c)

target_include_directories(open_file_dialog PRIVATE ../../modules/tinyfiledialogs)
