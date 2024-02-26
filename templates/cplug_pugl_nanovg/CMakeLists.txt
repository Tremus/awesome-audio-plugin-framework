cmake_minimum_required(VERSION 3.7)
project(template_cplug_pugl_nanovg VERSION 1.0.0)

set(CMAKE_C_STANDARD 99)

if (APPLE)
    enable_language(OBJC)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_STANDARD 20) # required for c++ clap build
    add_compile_options(
        /FI ${PROJECT_SOURCE_DIR}/config.h
    )
else()
    add_compile_options(
        -Werror=return-type
        -Werror=shadow
        -Wunused-function

        -Wno-deprecated
        -Wno-multichar
        -Wno-nullability-completeness
        -Wno-writable-strings

        -include${PROJECT_SOURCE_DIR}/src/config.h
    )
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-Werror=excess-initializers)
    endif()
endif()

set(HOTRELOAD_LIB_NAME template_cplug_pugl_nanovg_hotreload)
file(RELATIVE_PATH project_rel_path ${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR})

if (WIN32 AND CMAKE_BUILD_TYPE MATCHES Debug)
    add_library(${HOTRELOAD_LIB_NAME} MODULE src/plugin.c)
    target_include_directories(${HOTRELOAD_LIB_NAME} PRIVATE ../../modules/CPLUG/src/)
    add_executable(template_cplug_pugl_nanovg_standalone WIN32 ../../modules/CPLUG/src/cplug_standalone_win.c)
    target_include_directories(template_cplug_pugl_nanovg_standalone PRIVATE ../../modules/CPLUG/src/)

    # Windows paths are complicated
    set(HOTRELOAD_WATCH_DIR_POSIX "${PROJECT_SOURCE_DIR}/src")
    set(HOTRELOAD_LIB_PATH_POSIX "${CMAKE_BINARY_DIR}/${project_rel_path}/${HOTRELOAD_LIB_NAME}.dll")
    string(REPLACE "/" "\\\\" HOTRELOAD_WATCH_DIR "${HOTRELOAD_WATCH_DIR_POSIX}")
    string(REPLACE "/" "\\\\" HOTRELOAD_LIB_PATH "${HOTRELOAD_LIB_PATH_POSIX}")
    
    target_compile_definitions(${HOTRELOAD_LIB_NAME} PRIVATE -DCPLUG_SHARED)

    target_compile_definitions(template_cplug_pugl_nanovg_standalone PRIVATE
        -DHOTRELOAD_WATCH_DIR="${HOTRELOAD_WATCH_DIR}"
        -DHOTRELOAD_LIB_PATH="${HOTRELOAD_LIB_PATH}"
        -DHOTRELOAD_BUILD_COMMAND="cmake --build ${CMAKE_BINARY_DIR} --config Debug --target ${HOTRELOAD_LIB_NAME}"
        -DCPLUG_SHARED
        )
    add_dependencies(template_cplug_pugl_nanovg_standalone ${HOTRELOAD_LIB_NAME})
elseif (APPLE)
    # unfortunately mtl nanovg was written using ARC
    add_library(nanovg_mtl ../../modules/nanovg_compat/src/nanovg_compat.m)
    target_include_directories(nanovg_mtl PRIVATE
        ../../modules/nanovg_compat/modules/MetalNanoVG/src
        ../../modules/nanovg_compat/modules/nanovg_dx11/src)
    target_compile_options(nanovg_mtl PRIVATE "-fobjc-arc")
    # plugin
    add_library(${HOTRELOAD_LIB_NAME} MODULE
        src/plugin.c
        ../../modules/pugl/src/mac.m
        ../../modules/pugl/src/mac_stub.m
        ../../modules/pugl/src/internal.c
        ../../modules/pugl/src/common.c
        ../../modules/nanovg_compat/modules/nanovg_dx11/src/nanovg.c
        )
    target_include_directories(${HOTRELOAD_LIB_NAME}
        PRIVATE ../../modules/CPLUG/src/
        ../../modules/pugl/include
        ../../modules/nanovg_compat/src
        ../../modules/nanovg_compat/modules/MetalNanoVG/src
        ../../modules/nanovg_compat/modules/nanovg_dx11/src
        )
    target_link_libraries(${HOTRELOAD_LIB_NAME} PRIVATE nanovg_mtl "-framework Cocoa -framework CoreVideo -framework Metal -framework Quartz")

    # wrapper
    add_executable(template_cplug_pugl_nanovg_app MACOSX_BUNDLE
        ../../modules/CPLUG/src/cplug_standalone_osx.m
        )
    target_include_directories(template_cplug_pugl_nanovg_app PRIVATE
        ../../modules/CPLUG/src)

    set_target_properties(template_cplug_pugl_nanovg_app PROPERTIES
        BUNDLE True
        OUTPUT_NAME "template_cplug_pugl_nanovg"                                           # out binary name, differs from target name
        CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.app  # out directory name, differs from target name
        MACOSX_BUNDLE_GUI_IDENTIFIER com.aapf.${PROJECT_NAME}.app
        MACOSX_BUNDLE_BUNDLE_NAME "CPLUG pugl template"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
        MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/src/app.plist.in
    )
    file(TOUCH_NOCREATE ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.app/Contents/PkgInfo)
    file(WRITE ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.app/Contents/PkgInfo "APPL????")
    target_link_libraries(template_cplug_pugl_nanovg_app PRIVATE "-framework Cocoa -framework CoreMIDI -framework CoreAudio -framework CoreServices")

    target_compile_definitions(template_cplug_pugl_nanovg_app PRIVATE
        -DHOTRELOAD_WATCH_DIR="${PROJECT_SOURCE_DIR}/src"
        -DHOTRELOAD_LIB_PATH="${CMAKE_BINARY_DIR}/${project_rel_path}/lib${HOTRELOAD_LIB_NAME}.so"
        -DHOTRELOAD_BUILD_COMMAND="cmake --build ${CMAKE_BINARY_DIR} --config Debug --target ${HOTRELOAD_LIB_NAME}"
        -DCPLUG_SHARED
        )
    add_dependencies(template_cplug_pugl_nanovg_app ${HOTRELOAD_LIB_NAME})
endif()