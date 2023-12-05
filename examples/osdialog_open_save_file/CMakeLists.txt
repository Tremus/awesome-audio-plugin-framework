cmake_minimum_required(VERSION 3.15)

if (WIN32)
    set(CMAKE_C_STANDARD 99)
    add_executable(osdialog_open_save_file main.c)
elseif(APPLE)
    enable_language(OBJC)
    add_executable(osdialog_open_save_file main.m)
    target_link_libraries(osdialog_open_save_file "-framework Cocoa")
else()
    message(FATAL_ERROR "OS not supported")
endif()

target_include_directories(osdialog_open_save_file PRIVATE ../../modules/osdialog)