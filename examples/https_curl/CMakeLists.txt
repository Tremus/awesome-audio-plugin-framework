cmake_minimum_required(VERSION 3.15)
project(clap-c99-distortion VERSION 0.0.1 LANGUAGES C)
set(CMAKE_C_STANDARD 11)

add_executable(https_curl main.c)
target_include_directories(https_curl PUBLIC ../../modules/curl/include ../../modules/curl/lib)

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)
    target_link_libraries(https_curl PRIVATE "-framework Security -framework CoreFoundation -lz")
else()
    message(FATAL_ERROR "OS not supported yet")
endif()