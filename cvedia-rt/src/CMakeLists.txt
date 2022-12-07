# SPDX-FileCopyrightText: 2022 CVEDIA LTD
#
# SPDX-License-Identifier: Apache-2.0

# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.

# If install pref
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
    message(STATUS "Install dir not set, using default value.")
    set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/../redist/$<0:>)
endif()

if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY not set, using default value (CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}).")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<0:>)
endif()

if(NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY not set, using default value (CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}).")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<0:>)
endif()

if(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY not set, using default value (CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}).")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Plugins/$<0:>)
endif()

function(install2 _files)
    set (_resolvedFiles "")
    foreach (_file ${_files})
        get_filename_component(_resolvedPath "${_file}" DIRECTORY)
        get_filename_component(_resolvedFile "${_file}" NAME)
        file(GLOB SRC_FILES ${_resolvedPath}/${_resolvedFile}*)
        install(FILES ${SRC_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX})
    endforeach()    
endfunction()


cmake_minimum_required (VERSION 3.15)

if(WIN32)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CMakePathsWindows.txt")
        include("CMakePathsWindows.txt")
    else()
        message( FATAL_ERROR "${CMAKE_CURRENT_SOURCE_DIR}/CMakePathsWindows.txt file not found. Please create one according to the predefined template on README.md." )
    endif()
endif()

project ("CVEDIART")
  
include(CheckCXXSymbolExists)

# NOTE: Presets are expecting to everything being OFF by default!
option(WITH_TESTS "Build Tests" OFF)
option(DISCOVER_TESTS "Build Tests" OFF)
option(WITH_DOCS "Build docs" OFF)
option(ENABLE_CODE_COVERAGE "Build with code coverage" OFF)
option(ENABLE_NEON "Build with Neon compilation flags" OFF)
option(WITH_MODELFORGE "Build with ModelForge support" OFF)
option(WITH_VIDEOREADER_MODULE "Build VideoReader module" OFF)
option(WITH_UI "Build with UI support" OFF)
option(WITH_RENDERER_SDL "Build SDL renderer support" OFF)
option(WITH_RENDERER_GLFW "Build GLFW renderer support" OFF)
option(WITH_RENDERER_REMOTE "Build Remote renderer support" OFF)
option(WITH_COCO "Build with COCO Eval support" OFF)
option(WITH_OPENVINO_MODULE "Build OpenVino module" OFF)
option(WITH_HAILO_MODULE "Build Hailo module" OFF)
option(WITH_SIGMASTAR_MODULE "Build Sigmastar plugin" OFF)
option(WITH_SIGMASTAR_DLA_PLUGIN "Build Sigmastar DLA plugin" OFF)
option(WITH_SIGMASTAR_READER_PLUGIN "Build Sigmastar READER plugin" OFF)
option(WITH_ONNX_MODULE "Build ONNX module" OFF)
option(WITH_ARMNN_MODULE "Build ARMNN module" OFF)
option(WITH_MNN_MODULE "Build MNN module" OFF)
option(WITH_TRT_MODULE "Build with TensorRT module" OFF)
option(WITH_MQTT_MODULE "Build with MQTT module" OFF)
option(WITH_REST_MODULE "Build with REST module" OFF)
option(WITH_ZMQ_MODULE "Build with ZMQ module" OFF)
option(WITH_SCREENCAP_MODULE "Build with DX screencapture module" OFF)
option(WITH_GSTREAMERWRITER_MODULE "Build with GStreamer Writer module" OFF)
option(WITH_GSTREAMERREADER_MODULE "Build with GStreamer Reader module" OFF)
option(WITH_TRACKER_MODULE "Build with Tracker module" OFF)
option(WITH_TRIGGER_MODULE "Build with Trigger module" OFF)
option(WITH_MOTION_MODULE "Build with Motion module" OFF)
option(WITH_WRITEDATA_MODULE "Build with WriteData module" OFF)
option(WITH_JSONIO_MODULE "Build with JSONIO module" OFF)
option(WITH_VIDEO_STAB "Build with OpenCV video stabilization support" OFF)
option(WITH_GENERIC_KALMAN "Build with generic Kalman interface" OFF)
option(WITH_CAMERA_DISCOVERY "Build camera discovery support" OFF)
option(WITH_VERSION_CONTROL "Build with CValue version control support" OFF)
option(WITH_WEBSERVER "Build with built-in webserver" OFF)
option(WITH_ONVIF "Build with ONVIF support" OFF)
option(WITH_SOLUTIONS "Build with solution support" OFF)

option(BUILD_ARMNN_LIB "Build ARMNN from source" OFF)
option(BUILD_XTENSOR_LIB "Build XTensor from source" OFF)
option(BUILD_FLATBUFFERS_LIB "Build FlatBuffers from source" OFF)
option(BUILD_LUA_LIB "Build Lua from source" OFF)
option(BUILD_CPU_FEATURES_LIB "Build CpuFeatures from source" OFF)
option(CPPHTTPLIB_OPENSSL_SUPPORT "Build httplib with OpenSSL support" OFF)
option(SYSTEM_JETSON "NVIDIA Jetson system info" OFF)

option(WITH_CRTD_CLI "Build Cvedia RT UI CLI" OFF)
option(WITH_RUNINSTANCE_CLI "Build With Runinstance CLI" OFF)
option(WITH_RUNINFERENCE_CLI "Build With RunInference CLI" OFF)
option(WITH_LISTNNDEVICES_CLI "Build With ListNNDevices CLI" OFF)
option(WITH_GETDEVICECONFIG_CLI "Build With GetDeviceConfig CLI" OFF)
option(WITH_MODELFORGE_CLI "Build With ModelForge CLI" OFF)
option(WITH_BENCHMARK_CLI "Build With Benchmark CLI" OFF)
option(WITH_EVALUATE_CLI "Build With Evaluate CLI" OFF)
option(WITH_SYSTEMINFO_CLI "Build With SystemInfo CLI" OFF)
option(WITH_PRE_DEFAULT_MODULE "Build with pre-default module" OFF)
option(WITH_POST_CLASSIFIER_MODULE "Build with post-classifier module" OFF)
option(WITH_POST_PASSTHRU_MODULE "Build with post-passthru module" OFF)
option(WITH_POST_DISCARD_MODULE "Build with post-discard module" OFF)
option(WITH_POST_GFL_MODULE "Build with post-gfl module" OFF)
option(WITH_POST_ULTRAFACE_MODULE "Build with post-ultraface module" OFF)
option(WITH_POST_YOLOV4_MODULE "Build with post-yolov4 module" OFF)
option(WITH_POST_YOLOX_MODULE "Build with post-yolox module" OFF)
option(WITH_POST_POINT_MODULE "Build with post-point module" OFF)
option(WITH_POST_SAVE_MODULE "Build with post-save module" OFF)
option(WITH_IMAGEREADER_MODULE "Build with imagereader module" OFF)
option(WITH_POST_NEURALSTYLE_MODULE "Build with post-neuralstyle module" OFF)

if (WITH_TESTS)
    enable_testing()
endif()

set(CMAKE_SHARED_LIBRARY_PREFIX "")

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

  # make sure config.h can be found
include_directories("${PROJECT_BINARY_DIR}")

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS OFF)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

if (NOT DEFINED CVEDIA_BIN_ARCH)
    set(CVEDIA_BIN_ARCH "x86_64")
endif()

if (NOT DEFINED CVEDIA_BIN_VERSION)
    set(CVEDIA_BIN_VERSION "2022.11.0")
endif()

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/../lib")
    message(STATUS "Downloading CVEDIA-RT SDK libraries...")
    execute_process(COMMAND wget https://bin.cvedia.com/${CVEDIA_BIN_VERSION}/sdk-lib.tar.gz -O /tmp/file.tar.gz COMMAND_ERROR_IS_FATAL ANY)
    execute_process(COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/../lib COMMAND_ERROR_IS_FATAL ANY)
    execute_process(COMMAND tar -vxf /tmp/file.tar.gz -C ${CMAKE_SOURCE_DIR}/../lib COMMAND_ERROR_IS_FATAL ANY)
    execute_process(COMMAND rm -f /tmp/file.tar.gz)
endif()

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/../bin")
    message(STATUS "Downloading CVEDIA-RT SDK binaries...")
    execute_process(COMMAND wget https://bin.cvedia.com/${CVEDIA_BIN_VERSION}/sdk-bin.tar.gz -O /tmp/file.tar.gz COMMAND_ERROR_IS_FATAL ANY)
    execute_process(COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/../bin COMMAND_ERROR_IS_FATAL ANY)
    execute_process(COMMAND tar -vxf /tmp/file.tar.gz -C ${CMAKE_SOURCE_DIR}/../bin COMMAND_ERROR_IS_FATAL ANY)
    execute_process(COMMAND rm -f /tmp/file.tar.gz)
endif()

link_directories("../../RTCORE/src/" "${CMAKE_SOURCE_DIR}/../lib/${CVEDIA_BIN_ARCH}/" "${CMAKE_SOURCE_DIR}/../bin/${CVEDIA_BIN_ARCH}/")

IF(UNIX)
    set(OS_NAME "Linux")
ENDIF(UNIX)

IF(WIN32 AND (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    set(OS_NAME "../${COMPILER_CONFIG}")
    # Don't show warnings for any #include <> headers
    add_compile_options(/external:anglebrackets /external:W0)
ENDIF()

if (NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message(WARNING "CMAKE_BUILD_TYPE is not defined. Using default value: RelWithDebInfo")
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build arch: ${CMAKE_HOST_SYSTEM_PROCESSOR}")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# DO NOT CHANGE. C++14 allows compatibility with GCC5.x
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

# NOTE: CMAKE_CXX_FLAGS shall not influence check_cxx_symbol_exists, but it does ಠ_ಠ
set(CMAKE_CXX_FLAGS_T "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
if ((CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    set(HAVE_SHARED_MUTEX 1)
else()
    check_cxx_symbol_exists("std::shared_mutex()" shared_mutex HAVE_SHARED_MUTEX)
    check_cxx_symbol_exists("std::shared_timed_mutex()" shared_mutex HAVE_SHARED_TIMED_MUTEX)
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_T}")
unset(CMAKE_CXX_FLAGS_T)

# Maximum warnings and treat them as errors
if (MSVC)
    add_compile_options(/DNOMINMAX)
	
    if (NOT (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
        add_compile_options(/MP)
	else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat-pedantic -Wno-conversion -Wno-exit-time-destructors -Wno-documentation")
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG -fstack-protector-all -Wstack-protector --param ssp-buffer-size=4 -Wall -Wformat-security -Wextra -Wno-missing-braces -ftemplate-depth=2000")

    # enable colored outputs
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options (-fcolor-diagnostics)
    endif ()

    if(ENABLE_CODE_COVERAGE)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage")
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage")
    elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        message(STATUS "Using Release CXX flags")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g0 -O3 -s")
    elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
        message(STATUS "Using RelWithDebInfo CXX flags")
        #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O3 -march=native")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        message(STATUS "Using Debug CXX flags")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -g3 -O0 -fsanitize=address")
    else()
        message(FATAL_ERROR "No specific CXX flags set for build type ${CMAKE_BUILD_TYPE}")
    endif()
endif()

# tell CMake to generate config.h
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/rtconfig.h"
  )

include_directories(${CMAKE_SOURCE_DIR}/RTCORE/include ${CMAKE_SOURCE_DIR}/3rdparty/libs/ ${CMAKE_SOURCE_DIR}/3rdparty/Lua/src ${CMAKE_SOURCE_DIR}/3rdparty/xtensor/include ${CMAKE_SOURCE_DIR}/3rdparty/xtl/include ${CMAKE_SOURCE_DIR}/3rdparty/ ${CMAKE_SOURCE_DIR}/3rdparty/imgui ${CMAKE_SOURCE_DIR}/3rdparty/cpu_features/include)

# Include sub-projects.
add_subdirectory ("cli")
add_subdirectory ("Plugins")
add_subdirectory ("3rdparty")

# if (WITH_DOCS)
#     if (UNIX)
#         set(DOXYBOOK_FILE_PATH /usr/local/bin/doxybook2)
#         set(DOXYBOOK_CONFIG /doxybook_config.json)
#     endif()

#     message(STATUS "Building with docs")
#     add_subdirectory ("Doxygen")
# endif()

# if (WITH_TESTS)
#     message(STATUS "Building with tests")
#    add_subdirectory ("tests")
# endif()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
