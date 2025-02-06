project(template_cplug_window_nanovg
    VERSION 1.0.0
    DESCRIPTION "Project template using CPLUG & NanoVG"
    LANGUAGES C)
set(CMAKE_C_STANDARD 99)

if (APPLE)
    enable_language(OBJC)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

if (CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    add_compile_options(/W3 /FI ${CMAKE_CURRENT_SOURCE_DIR}/plugin.h)
else()
    add_compile_options(-include${CMAKE_CURRENT_SOURCE_DIR}/plugin.h)
endif()
if (NOT ${CMAKE_C_COMPILER_ID} STREQUAL "MSVC")
add_compile_options(
    -Werror=return-type
    -Werror=shadow
    -Wunused-function

    -Wno-deprecated
    -Wno-multichar
    -Wno-nullability-completeness
    -Wno-writable-strings
    -Wno-microsoft-enum-forward-reference
    )
endif()

if (WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

file(RELATIVE_PATH project_rel_path ${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR})
set(OUTPUT_PATH ${CMAKE_BINARY_DIR}/${project_rel_path})

set(PRODUCT_NAME "CPLUG + NanoVG")
set(PRODUCT_COPYRIGHT AAPG)
set(PLUGIN_BUNDLE_ID "com.aapf.windownanovg.plugin")
set(APP_BUNDLE_ID "com.aapf.windownanovg.app")
# Full list of AU types & tags here:
# https://developer.apple.com/library/archive/documentation/General/Conceptual/ExtensibilityPG/AudioUnit.html
# The properties:
set(MACOSX_BUNDLE_TAGS "<string>Effects</string>") # Predefined tags are: Bass, Delay, Distortion, Drums, Dynamics, Dynamics Processor, Effects, Equalizer, Filter, Format Converter, Guitar, Imaging, MIDI, Mixer, Offline Effect, Output, Panner, Pitch, Reverb, Sampler, Synthesizer, Time Effect, Vocal. But you can use others.
set(MACOSX_BUNDLE_TYPE "aufx") # "aufx" for Effect, "augn" for Generator, "aumu" for Instrument, "aufm" for Music Effect.
set(MACOSX_BUNDLE_SUBTYPE "Xmpl") # For AU and GarageBand 10.3 compatibility, the first letter must be upper-case, the others lower-case.
set(MACOSX_BUNDLE_MANUFACTURER "Aapf") # For AU and GarageBand 10.3 compatibility, the first letter must be upper-case, the others lower-case.
# Convert version string (1.0.0) > integer (65536)
math(EXPR MACOSX_BUNDLE_VERSION_INT "${PROJECT_VERSION_MAJOR} << 16 | ${PROJECT_VERSION_MINOR} << 8 | ${PROJECT_VERSION_PATCH}" OUTPUT_FORMAT DECIMAL) # cool trick bro

set(PLUGIN_SOURCES
    plugin.c
    ${CMAKE_SOURCE_DIR}/modules/nanovg_compat/src/nanovg_compat.c
)
set(PLUGIN_INCLUDES
    ${CMAKE_SOURCE_DIR}/modules/CPLUG/src/
    ${CMAKE_SOURCE_DIR}/modules/nanovg_compat/src
    ${CMAKE_SOURCE_DIR}/modules/nanovg_compat/modules/nanovg_dx11/src
    ${CMAKE_SOURCE_DIR}/modules/nanovg_compat/modules/MetalNanoVG/src
)
set(PLUGIN_LIBRARIES "")
if (APPLE)
    list(APPEND PLUGIN_SOURCES
        ${CMAKE_SOURCE_DIR}/modules/CPLUG/src/cplug_extensions/window_osx.m
        )

    # unfortunately mtl nanovg was written using ARC
    add_library(cplug_window_nanovg_mtl ${CMAKE_SOURCE_DIR}/modules/nanovg_compat/src/nanovg_compat.m)
    target_include_directories(cplug_window_nanovg_mtl PRIVATE
        ${CMAKE_SOURCE_DIR}/modules/nanovg_compat/modules/MetalNanoVG/src
        ${CMAKE_SOURCE_DIR}/modules/nanovg_compat/modules/nanovg_dx11/src)
    target_compile_options(cplug_window_nanovg_mtl PRIVATE "-fobjc-arc")

    list(APPEND PLUGIN_LIBRARIES
        cplug_window_nanovg_mtl
        "-framework Cocoa"
        "-framework QuartzCore"
        "-framework Metal"
        "-framework MetalKit"
        )
elseif(WIN32)
    list(APPEND PLUGIN_SOURCES
        ${CMAKE_SOURCE_DIR}/modules/CPLUG/src/cplug_extensions/window_win.c
        )
    list(APPEND PLUGIN_LIBRARIES d3d11 dxguid)
endif()

# Params @bundle_identifier@ and @bundle_type@ are used by the plist
function(configure_info_plist target_name bundle_identifier bundle_type bundle_extension)
    # Generate plist file
    configure_file(
        ${CMAKE_SOURCE_DIR}/modules/CPLUG/example/Info.plist.in
        ${OUTPUT_PATH}/${target_name}_${bundle_type}.plist
        @ONLY
    )

    set_target_properties(${target_name} PROPERTIES
        BUNDLE True
        MACOSX_BUNDLE TRUE
        OUTPUT_NAME ${PROJECT_NAME}
        BUNDLE_EXTENSION ${bundle_extension}
        MACOSX_BUNDLE_INFO_PLIST ${OUTPUT_PATH}/${target_name}_${bundle_type}.plist
        CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH}/${PROJECT_NAME}.${bundle_extension}
    )
endfunction()

# ██████╗ ██╗     ██╗   ██╗ ██████╗ ██╗███╗   ██╗
# ██╔══██╗██║     ██║   ██║██╔════╝ ██║████╗  ██║
# ██████╔╝██║     ██║   ██║██║  ███╗██║██╔██╗ ██║
# ██╔═══╝ ██║     ██║   ██║██║   ██║██║██║╚██╗██║
# ██║     ███████╗╚██████╔╝╚██████╔╝██║██║ ╚████║
# ╚═╝     ╚══════╝ ╚═════╝  ╚═════╝ ╚═╝╚═╝  ╚═══╝

# All Plugin formats in one
if (APPLE)
    add_library(${PROJECT_NAME}_plugin MODULE
        ${PLUGIN_SOURCES}
        ${CMAKE_SOURCE_DIR}/modules/CPLUG/src/cplug_auv2.c
        ${CMAKE_SOURCE_DIR}/modules/CPLUG/src/cplug_clap.c
        ${CMAKE_SOURCE_DIR}/modules/CPLUG/src/cplug_vst3.c
        )
    target_link_libraries(${PROJECT_NAME}_plugin PRIVATE ${PLUGIN_LIBRARIES} "-framework AudioToolbox") # -framework AudioToolbox not actually required...

    configure_info_plist(${PROJECT_NAME}_plugin ${PLUGIN_BUNDLE_ID} "BNDL" "clap")

    file(TOUCH_NOCREATE "${OUTPUT_PATH}/${PROJECT_NAME}.clap/Contents/PkgInfo")
    file(WRITE "${OUTPUT_PATH}/${PROJECT_NAME}.clap/Contents/PkgInfo" "BNDL????")

    target_compile_definitions(${PROJECT_NAME}_plugin PRIVATE
        CPLUG_AUV2_VERSION_INT=${MACOSX_BUNDLE_VERSION_INT}
        CPLUG_AUV2_BUNDLE_ID="${PLUGIN_BUNDLE_ID}"
        CPLUG_BUILD_AUV2=1
    )

    add_custom_command(TARGET ${PROJECT_NAME}_plugin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${OUTPUT_PATH}/${PROJECT_NAME}.vst3"
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${OUTPUT_PATH}/${PROJECT_NAME}.component"

        COMMAND ${CMAKE_COMMAND} -E copy_directory "${OUTPUT_PATH}/${PROJECT_NAME}.clap" "${OUTPUT_PATH}/${PROJECT_NAME}.vst3"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${OUTPUT_PATH}/${PROJECT_NAME}.clap" "${OUTPUT_PATH}/${PROJECT_NAME}.component"

        COMMAND ${CMAKE_COMMAND} -E echo "Installing ${OUTPUT_PATH}/${PROJECT_NAME}.clap to ~/Library/Audio/Plug-Ins/CLAP/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${OUTPUT_PATH}/${PROJECT_NAME}.clap" "~/Library/Audio/Plug-Ins/CLAP/${PROJECT_NAME}.clap"

        COMMAND ${CMAKE_COMMAND} -E echo "Installing ${OUTPUT_PATH}/${PROJECT_NAME}.vst3 to ~/Library/Audio/Plug-Ins/VST3/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${OUTPUT_PATH}/${PROJECT_NAME}.vst3" "~/Library/Audio/Plug-Ins/VST3/${PROJECT_NAME}.vst3"

        COMMAND ${CMAKE_COMMAND} -E echo "Installing ${OUTPUT_PATH}/${PROJECT_NAME}.component to ~/Library/Audio/Plug-Ins/Components/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${OUTPUT_PATH}/${PROJECT_NAME}.component" "~/Library/Audio/Plug-Ins/Components/${PROJECT_NAME}.component"

    )
elseif(WIN32)
    add_library(${PROJECT_NAME}_plugin MODULE ${PLUGIN_SOURCES}
        ${CMAKE_SOURCE_DIR}/modules/CPLUG/src/cplug_clap.c
        ${CMAKE_SOURCE_DIR}/modules/CPLUG/src/cplug_vst3.c)
    target_link_libraries(${PROJECT_NAME}_plugin PRIVATE ${PLUGIN_LIBRARIES})
    set_target_properties(${PROJECT_NAME}_plugin PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME}"
        SUFFIX .clap
        PDB_NAME "${PROJECT_NAME}"
        )
    add_custom_command(TARGET ${PROJECT_NAME}_plugin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${OUTPUT_PATH}/${PROJECT_NAME}.vst3"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_PATH}/${PROJECT_NAME}.vst3/Contents/x86_64-win"
        COMMAND ${CMAKE_COMMAND} -E copy "${OUTPUT_PATH}/${PROJECT_NAME}.clap" "${OUTPUT_PATH}/${PROJECT_NAME}.vst3/Contents/x86_64-win/${PROJECT_NAME}.vst3"
        )
endif()
target_include_directories(${PROJECT_NAME}_plugin PRIVATE ${PLUGIN_INCLUDES})


# ███████╗████████╗ █████╗ ███╗   ██╗██████╗  █████╗ ██╗      ██████╗ ███╗   ██╗███████╗
# ██╔════╝╚══██╔══╝██╔══██╗████╗  ██║██╔══██╗██╔══██╗██║     ██╔═══██╗████╗  ██║██╔════╝
# ███████╗   ██║   ███████║██╔██╗ ██║██║  ██║███████║██║     ██║   ██║██╔██╗ ██║█████╗  
# ╚════██║   ██║   ██╔══██║██║╚██╗██║██║  ██║██╔══██║██║     ██║   ██║██║╚██╗██║██╔══╝  
# ███████║   ██║   ██║  ██║██║ ╚████║██████╔╝██║  ██║███████╗╚██████╔╝██║ ╚████║███████╗
# ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝ ╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

if (WIN32)
    add_executable(${PROJECT_NAME}_app WIN32
        ${PLUGIN_SOURCES}
        ${CMAKE_SOURCE_DIR}/modules/CPLUG/src/cplug_standalone_win.c
        )
    target_link_libraries(${PROJECT_NAME}_app PRIVATE ${PLUGIN_LIBRARIES})
elseif (APPLE)
    add_executable(${PROJECT_NAME}_app MACOSX_BUNDLE
        ${PLUGIN_SOURCES}
        ${CMAKE_SOURCE_DIR}/modules/CPLUG/src/cplug_standalone_osx.m
        )
    target_compile_definitions(${PROJECT_NAME}_app PRIVATE PW_METAL)
    target_link_libraries(${PROJECT_NAME}_app PRIVATE ${PLUGIN_LIBRARIES} "-framework CoreMIDI -framework CoreAudio")
    configure_info_plist(${PROJECT_NAME}_app ${APP_BUNDLE_ID} "APPL" "app")

    file(TOUCH_NOCREATE "${OUTPUT_PATH}/${PROJECT_NAME}.app/Contents/PkgInfo")
    file(WRITE "${OUTPUT_PATH}/${PROJECT_NAME}.app/Contents/PkgInfo" "APPL????")
endif()

target_include_directories(${PROJECT_NAME}_app PRIVATE ${PLUGIN_INCLUDES})