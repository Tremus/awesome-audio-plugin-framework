cmake_minimum_required(VERSION 3.15)

add_executable(save_file_dialog save_file_dialog.c)

target_include_directories(save_file_dialog PRIVATE ../../modules/tinyfiledialogs)
