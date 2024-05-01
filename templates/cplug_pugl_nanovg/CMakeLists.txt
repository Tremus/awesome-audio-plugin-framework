cmake_minimum_required(VERSION 3.7)
project(template_cplug_pugl_nanovg VERSION 1.0.0)

set(CMAKE_C_STANDARD 99)

if (MSVC)
    set(CMAKE_CXX_STANDARD 20) # required for c++ clap build
    add_compile_options(
        /FI ${PROJECT_SOURCE_DIR}/src/config.h
        /DPUGL_STATIC
    )
else()
    add_compile_options(-include${PROJECT_SOURCE_DIR}/src/config.h)
endif()
if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        -Werror=return-type
        -Werror=shadow
        -Wunused-function

        -Wno-deprecated
        -Wno-multichar
        -Wno-nullability-completeness
        -Wno-writable-strings

        -DPUGL_STATIC
        -Werror=excess-initializers)
endif()

set(PLUGIN_SOURCES
    src/plugin.c
    ../../modules/pugl/src/internal.c
    ../../modules/pugl/src/common.c
)
set(PLUGIN_INCLUDE
    ../../modules/CPLUG/src/
    ../../modules/pugl/include
    ../../modules/nanovg_compat/src
    ../../modules/nanovg_compat/modules/nanovg_dx11/src)
set(PLUGIN_LIBRARIES "")

if (WIN32)
    list(APPEND PLUGIN_SOURCES
        ../../modules/pugl/src/win.c
        ../../modules/pugl/src/win_stub.c
        ../../modules/nanovg_compat/src/nanovg_compat.c)
    list(APPEND PLUGIN_LIBRARIES d3d11 dxguid Dwmapi)
elseif(APPLE)
    enable_language(OBJC)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")

    # unfortunately mtl nanovg was written using ARC
    add_library(nanovg_mtl ../../modules/nanovg_compat/src/nanovg_compat.m)
    target_include_directories(nanovg_mtl PRIVATE
        ../../modules/nanovg_compat/modules/MetalNanoVG/src
        ../../modules/nanovg_compat/modules/nanovg_dx11/src)
    target_compile_options(nanovg_mtl PRIVATE "-fobjc-arc")

    list(APPEND PLUGIN_SOURCES
        ../../modules/pugl/src/mac.m
        ../../modules/pugl/src/mac_stub.m
        ../../modules/nanovg_compat/modules/nanovg_dx11/src/nanovg.c)
    list(APPEND PLUGIN_INCLUDE ../../modules/nanovg_compat/modules/MetalNanoVG/src)
    list(APPEND PLUGIN_LIBRARIES
        nanovg_mtl
        "-framework Cocoa"
        "-framework CoreVideo"
        "-framework Metal"
        "-framework Quartz")
endif()

# ███████╗████████╗ █████╗ ███╗   ██╗██████╗  █████╗ ██╗      ██████╗ ███╗   ██╗███████╗
# ██╔════╝╚══██╔══╝██╔══██╗████╗  ██║██╔══██╗██╔══██╗██║     ██╔═══██╗████╗  ██║██╔════╝
# ███████╗   ██║   ███████║██╔██╗ ██║██║  ██║███████║██║     ██║   ██║██╔██╗ ██║█████╗  
# ╚════██║   ██║   ██╔══██║██║╚██╗██║██║  ██║██╔══██║██║     ██║   ██║██║╚██╗██║██╔══╝  
# ███████║   ██║   ██║  ██║██║ ╚████║██████╔╝██║  ██║███████╗╚██████╔╝██║ ╚████║███████╗
# ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝ ╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝


set(HOTRELOAD_LIB_NAME template_cplug_pugl_nanovg_hotreload)
file(RELATIVE_PATH project_rel_path ${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR})

if (WIN32 AND CMAKE_BUILD_TYPE MATCHES Debug)
    # plugin
    add_library(${HOTRELOAD_LIB_NAME} MODULE ${PLUGIN_SOURCES})
    target_include_directories(${HOTRELOAD_LIB_NAME} PRIVATE ${PLUGIN_INCLUDE})
    target_compile_definitions(${HOTRELOAD_LIB_NAME} PRIVATE CPLUG_SHARED)
    target_link_libraries(${HOTRELOAD_LIB_NAME} PRIVATE ${PLUGIN_LIBRARIES})

    # wrapper
    add_executable(template_cplug_pugl_nanovg_standalone WIN32 ../../modules/CPLUG/src/cplug_standalone_win.c)
    target_include_directories(template_cplug_pugl_nanovg_standalone PRIVATE ../../modules/CPLUG/src/)

    # Windows paths are complicated
    set(HOTRELOAD_WATCH_DIR_POSIX "${PROJECT_SOURCE_DIR}/src")
    set(HOTRELOAD_LIB_PATH_POSIX "${CMAKE_BINARY_DIR}/${project_rel_path}/${HOTRELOAD_LIB_NAME}.dll")
    string(REPLACE "/" "\\\\" HOTRELOAD_WATCH_DIR "${HOTRELOAD_WATCH_DIR_POSIX}")
    string(REPLACE "/" "\\\\" HOTRELOAD_LIB_PATH "${HOTRELOAD_LIB_PATH_POSIX}")
    
    target_compile_definitions(template_cplug_pugl_nanovg_standalone PRIVATE
        -DHOTRELOAD_WATCH_DIR="${HOTRELOAD_WATCH_DIR}"
        -DHOTRELOAD_LIB_PATH="${HOTRELOAD_LIB_PATH}"
        -DHOTRELOAD_BUILD_COMMAND="cmake --build ${CMAKE_BINARY_DIR} --config Debug --target ${HOTRELOAD_LIB_NAME}"
        -DCPLUG_SHARED
        )
    add_dependencies(template_cplug_pugl_nanovg_standalone ${HOTRELOAD_LIB_NAME})
elseif (APPLE)
    # plugin
    add_library(${HOTRELOAD_LIB_NAME} MODULE ${PLUGIN_SOURCES})
    target_include_directories(${HOTRELOAD_LIB_NAME} PRIVATE ${PLUGIN_INCLUDE})
    target_link_libraries(${HOTRELOAD_LIB_NAME} PRIVATE ${PLUGIN_LIBRARIES})

    # wrapper
    add_executable(template_cplug_pugl_nanovg_app MACOSX_BUNDLE
        ../../modules/CPLUG/src/cplug_standalone_osx.m
        )
    target_include_directories(template_cplug_pugl_nanovg_app PRIVATE
        ../../modules/CPLUG/src)

    set_target_properties(template_cplug_pugl_nanovg_app PROPERTIES
        BUNDLE True
        OUTPUT_NAME "template_cplug_pugl_nanovg"                                                               # out binary name, differs from target name
        CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.app  # out directory name, differs from target name
        MACOSX_BUNDLE_GUI_IDENTIFIER com.aapf.${PROJECT_NAME}.app
        MACOSX_BUNDLE_BUNDLE_NAME "CPLUG pugl template"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
        MACOSX_BUNDLE_COPYRIGHT AAPF
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

# ██╗   ██╗███████╗████████╗██████╗ 
# ██║   ██║██╔════╝╚══██╔══╝╚════██╗
# ██║   ██║███████╗   ██║    █████╔╝
# ╚██╗ ██╔╝╚════██║   ██║    ╚═══██╗
#  ╚████╔╝ ███████║   ██║   ██████╔╝
#   ╚═══╝  ╚══════╝   ╚═╝   ╚═════╝ 

add_library(template_cplug_pugl_nanovg_vst3 MODULE ${PLUGIN_SOURCES} ../../modules/CPLUG/src/cplug_vst3.c)
target_include_directories(template_cplug_pugl_nanovg_vst3 PRIVATE ${PLUGIN_INCLUDE})
target_link_libraries(template_cplug_pugl_nanovg_vst3 PRIVATE ${PLUGIN_LIBRARIES})

if (WIN32)
    # According to the docs you're meant to bundle your Windows VST3 using a bundle like folder structure:
    # https://steinbergmedia.github.io/vst3_dev_portal/pages/Technical+Documentation/Locations+Format/Plugin+Format.html#for-the-windows-platform
    # However, placing the unbundled binary directly into your ...\Common Files\VST3 folder will work fine in every host.
    # I'm not sure anyone in the real world actually bundles Windows VST3s like this, but maybe I'm just ignorant.
    set_target_properties(template_cplug_pugl_nanovg_vst3 PROPERTIES
        BUNDLE True
        BUNDLE_EXTENSION vst3
        OUTPUT_NAME template_cplug_pugl_nanovg                                                                 # out binary name, differs from target name
        CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.vst3 # out directory name, differs from target name
        SUFFIX .vst3
        PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.vst3
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.vst3/Contents/x86_64-win
    )
elseif (APPLE)
    set_target_properties(template_cplug_pugl_nanovg_vst3 PROPERTIES
        BUNDLE True
        BUNDLE_EXTENSION vst3
        OUTPUT_NAME template_cplug_pugl_nanovg                                                                # out binary name, differs from target name
        CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.vst3 # out directory name, differs from target name
        MACOSX_BUNDLE_BUNDLE_NAME "CPLUG + Pugl + NanoVG Template"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
        MACOSX_BUNDLE_GUI_IDENTIFIER com.aapf.CplugPuglNanovgTemplate.vst3
        MACOSX_BUNDLE_COPYRIGHT AAPF
        MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/src/vst3.plist.in
    )
    file(TOUCH_NOCREATE ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.vst3/Contents/PkgInfo)
    file(WRITE ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.vst3/Contents/PkgInfo "BNDL????")
    add_custom_command(TARGET template_cplug_pugl_nanovg_vst3 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Installing ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.vst3 to ~/Library/Audio/Plug-Ins/VST3/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.vst3" "~/Library/Audio/Plug-Ins/VST3/template_cplug_pugl_nanovg.vst3"
        )
endif()


#  █████╗ ██╗   ██╗██╗   ██╗██████╗ 
# ██╔══██╗██║   ██║██║   ██║╚════██╗
# ███████║██║   ██║██║   ██║ █████╔╝
# ██╔══██║██║   ██║╚██╗ ██╔╝██╔═══╝ 
# ██║  ██║╚██████╔╝ ╚████╔╝ ███████╗
# ╚═╝  ╚═╝ ╚═════╝   ╚═══╝  ╚══════╝

# Read the comments here carefully. Building Audio Units v2 through CMake is not as easy as VST3 or CLAP

if (APPLE)

add_library(template_cplug_pugl_nanovg_auv2 MODULE ${PLUGIN_SOURCES} ../../modules/CPLUG/src/cplug_auv2.c)
target_include_directories(template_cplug_pugl_nanovg_auv2 PRIVATE ${PLUGIN_INCLUDE})
target_link_libraries(template_cplug_pugl_nanovg_auv2 PRIVATE ${PLUGIN_LIBRARIES} "-framework AudioToolbox")

# The following properties will end up in our bundles .plist, but must be set here.
# CMake doesn't support passing them as arguments in 'set_target_properties' below
# Some of these properties are used by both the .plist, and inside the AU when the host requests a 'ClassInfo' dict
# I would love to only define these in src/config.h, but then I would be duplicating these properties.
# Instead, I configure all AU settings from a single place
# Full list of AU types & tags here:
# https://developer.apple.com/library/archive/documentation/General/Conceptual/ExtensibilityPG/AudioUnit.html
# The properties:
set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.aapf.CplugPuglNanovgTemplate.auv2")
set(MACOSX_BUNDLE_DESCRIPTION "CPLUG + Pugl + NanoVG Template")
set(MACOSX_BUNDLE_TAGS "<string>Synthesizer</string>")
set(MACOSX_BUNDLE_TYPE "aumu")
set(MACOSX_BUNDLE_SUBTYPE "cpvg")
set(MACOSX_BUNDLE_MANUFACTURER "AAPF")
# Convert version string (1.0.0) > integer (65536)
math(EXPR MACOSX_BUNDLE_VERSION_INT "${PROJECT_VERSION_MAJOR} << 16 | ${PROJECT_VERSION_MINOR} << 8 | ${PROJECT_VERSION_PATCH}" OUTPUT_FORMAT DECIMAL) # cool trick bro

set_target_properties(template_cplug_pugl_nanovg_auv2 PROPERTIES
    BUNDLE True
    BUNDLE_EXTENSION component
    OUTPUT_NAME "template_cplug_pugl_nanovg"                                                                    # out binary name, differs from target name
    CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.component # out directory name, differs from target name
    MACOSX_BUNDLE_BUNDLE_NAME "CPLUG + Pugl + NanoVG Template"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_COPYRIGHT AAPF
    MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/src/auv2.plist.in
    )
file(TOUCH_NOCREATE ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.component/Contents/PkgInfo)
file(WRITE ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.component/Contents/PkgInfo "BNDL????")

target_compile_definitions(template_cplug_pugl_nanovg_auv2 PRIVATE
    CPLUG_AUV2_VERSION_INT=${MACOSX_BUNDLE_VERSION_INT}
    CPLUG_AUV2_BUNDLE_ID="${MACOSX_BUNDLE_GUI_IDENTIFIER}"
    CPLUG_BUILD_AUV2
)

# For some reason Audio Units don't work unless they're inside their expcted folder.
# If I can find a way to remove this silly dependancy I will
# Until then, better to copy test plugins to the users directory
add_custom_command(TARGET template_cplug_pugl_nanovg_auv2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Installing ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.component to ~/Library/Audio/Plug-Ins/Components/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.component" "~/Library/Audio/Plug-Ins/Components/template_cplug_pugl_nanovg.component"
    )

endif() # APPLE

#  ██████╗██╗      █████╗ ██████╗
# ██╔════╝██║     ██╔══██╗██╔══██╗
# ██║     ██║     ███████║██████╔╝
# ██║     ██║     ██╔══██║██╔═══╝
# ╚██████╗███████╗██║  ██║██║
#  ╚═════╝╚══════╝╚═╝  ╚═╝╚═╝

add_library(template_cplug_pugl_nanovg_clap MODULE ${PLUGIN_SOURCES} ../../modules/CPLUG/src/cplug_clap.c)
target_include_directories(template_cplug_pugl_nanovg_clap PRIVATE ${PLUGIN_INCLUDE})
target_link_libraries(template_cplug_pugl_nanovg_clap PRIVATE ${PLUGIN_LIBRARIES})
if(WIN32)
    set_target_properties(template_cplug_pugl_nanovg_clap PROPERTIES
        OUTPUT_NAME template_cplug_pugl_nanovg
        SUFFIX .clap
        PDB_NAME template_cplug_pugl_nanovg_clap
        )
elseif (APPLE)
    set_target_properties(template_cplug_pugl_nanovg_clap PROPERTIES
        BUNDLE True
        BUNDLE_EXTENSION clap
        OUTPUT_NAME "template_cplug_pugl_nanovg"                                           # out binary name, differs from target name
        CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.clap # out directory name, differs from target name
        MACOSX_BUNDLE_GUI_IDENTIFIER com.aapf.CplugPuglNanovgTemplate.clap
        MACOSX_BUNDLE_BUNDLE_NAME "CPLUG + Pugl + NanoVG Template"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_COPYRIGHT AAPF
        MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/src/clap.plist.in
        )
    file(TOUCH_NOCREATE ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.clap/Contents/PkgInfo)
    file(WRITE ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.clap/Contents/PkgInfo "BNDL????")

    add_custom_command(TARGET template_cplug_pugl_nanovg_clap POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Installing ${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.clap to ~/Library/Audio/Plug-Ins/CLAP/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/${project_rel_path}/template_cplug_pugl_nanovg.clap" "~/Library/Audio/Plug-Ins/CLAP/template_cplug_pugl_nanovg.clap"
        )
endif()