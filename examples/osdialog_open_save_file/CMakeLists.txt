cmake_minimum_required(VERSION 3.15)
set(CMAKE_C_STANDARD 99)

add_executable(osdialog_open_save_file main.c)
target_include_directories(osdialog_open_save_file PRIVATE ../../modules/osdialog)