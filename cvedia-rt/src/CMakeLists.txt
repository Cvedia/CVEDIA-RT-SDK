# SPDX-FileCopyrightText: 2022 CVEDIA LTD
#
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.2)

project ("CVEDIART")

include(CheckCXXSymbolExists)

# Allow the user to specify the path of CVEDIA_INSTALL_DIR
if (WIN32)
    set(CVEDIA_INSTALL_DIR "" CACHE PATH "Path to the CVEDIA install directory")

    if (NOT EXISTS ${CVEDIA_INSTALL_DIR}/files/RTCORE.dll)
        message(FATAL_ERROR "CVEDIA_INSTALL_DIR does not point to a valid installation of CVEDIA-RT (RTCORE.dll missing)")
    endif()
	
	set(CVEDIA_INSTALL_DIR ${CVEDIA_INSTALL_DIR}/files)
else()
    set(CVEDIA_INSTALL_DIR "/opt/cvedia-rt" CACHE PATH "Path to the CVEDIA install directory")

    if (NOT EXISTS ${CVEDIA_INSTALL_DIR}/RTCORE.so)
        message(FATAL_ERROR "CVEDIA_INSTALL_DIR does not point to a valid installation of CVEDIA-RT (RTCORE.so missing)")
    endif()
endif()

set(CMAKE_INSTALL_PREFIX ${CVEDIA_INSTALL_DIR})


option(WITH_MNN_MODULE "Build MNN module" OFF)
option(WITH_POST_YOLOV4_MODULE "Build with post-yolov4 module" ON)
option(WITH_POST_YOLOV5_MODULE "Build with post-yolov5 module" ON)
option(WITH_POST_YOLOV6_MODULE "Build with post-yolov6 module" ON)
option(WITH_POST_YOLOV7_MODULE "Build with post-yolov7 module" ON)
option(WITH_POST_YOLOX_MODULE "Build with post-yolox module" ON)
option(WITH_POST_CLASSIFIER_MODULE "Build with post-classifier module" ON)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

  # make sure config.h can be found
include_directories("${PROJECT_BINARY_DIR}")

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

include_directories(${CMAKE_SOURCE_DIR}/RTCORE/include ${CMAKE_SOURCE_DIR}/3rdparty/libs/ ${CMAKE_SOURCE_DIR}/3rdparty/xtensor/include ${CMAKE_SOURCE_DIR}/3rdparty/xtl/include ${CMAKE_SOURCE_DIR}/3rdparty/)

if (WIN32)
    add_library(RTCORE STATIC IMPORTED)
    set_property(TARGET RTCORE PROPERTY
                IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/../lib/win32/RTCORE.lib")
else()
    add_library(RTCORE STATIC IMPORTED)
    set_property(TARGET RTCORE PROPERTY
                IMPORTED_LOCATION "${CVEDIA_INSTALL_DIR}/RTCORE.so")
endif()

# Include sub-projects.
add_subdirectory ("Plugins")

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
