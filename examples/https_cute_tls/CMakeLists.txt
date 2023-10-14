cmake_minimum_required(VERSION 3.15)

if (APPLE)
    enable_language(OBJC)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.14)
    add_executable(https_cute_tls main.m)
    target_link_libraries(https_cute_tls "-framework Network")
endif()

target_include_directories(https_cute_tls PUBLIC ../../modules/cute_headers)