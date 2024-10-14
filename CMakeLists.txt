# SPDX-FileCopyrightText: 2022 CVEDIA LTD
#
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.23)

project ("CVEDIART")

include("cmake/Colors.cmake")
include("cmake/CompilerOptions.cmake")
include("cmake/CustomCommands.cmake")

include(FindPackageHandleStandardArgs)
include(FetchContent)
#include(CustomCommands)

set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_FULLY_DISCONNECTED OFF)

if (NOT CVEDIA_INSTALL_DIR STREQUAL "RTCORE")
    # Allow the user to specify the path of CVEDIA_INSTALL_DIR
    if (WIN32)
        if (NOT CVEDIA_INSTALL_DIR)    
            set(CVEDIA_INSTALL_DIR "" CACHE PATH "Path to the CVEDIA install directory")
        endif()

        message(STATUS "CVEDIA_INSTALL_DIR: ${CVEDIA_INSTALL_DIR}")

        if (NOT EXISTS ${CVEDIA_INSTALL_DIR}/RTCORE.lib)
            message(FATAL_ERROR "CVEDIA_INSTALL_DIR does not point to a valid installation of CVEDIA-RT (RTCORE.lib missing)")
        endif()
    else()
        if (NOT CVEDIA_INSTALL_DIR)
            set(CVEDIA_INSTALL_DIR "/opt/cvedia-rt" CACHE PATH "Path to the CVEDIA install directory")
        endif()

        message(STATUS "CVEDIA_INSTALL_DIR: ${CVEDIA_INSTALL_DIR}")

        if (NOT EXISTS ${CVEDIA_INSTALL_DIR}/RTCORE.so)
            message(FATAL_ERROR "CVEDIA_INSTALL_DIR does not point to a valid installation of CVEDIA-RT (RTCORE.so missing)")
        endif()
    endif()


    if (WIN32)
        set(RTCORELib ${CVEDIA_INSTALL_DIR}/RTCORE.lib)
    else()
        set(RTCORELib ${CVEDIA_INSTALL_DIR}/RTCORE.so)
    endif()
else()
    # use 'target' instead
    set(RTCORELib RTCORE)
endif()

message(STATUS "RTCORELib: ${RTCORELib}")

message("${Yellow}Configuring nlohmann::json${ColorReset}")
FetchContent_Declare(json 
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

message("${Yellow}Configuring plog${ColorReset}")
FetchContent_Declare(plog 
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    GIT_REPOSITORY https://github.com/SergiusTheBest/plog
    GIT_TAG 1.1.10
)
FetchContent_MakeAvailableNoInstall(plog)

message("${Yellow}Configuring XTL${ColorReset}")
FetchContent_Declare(xtl
    GIT_REPOSITORY https://github.com/xtensor-stack/xtl.git
    GIT_TAG 0.7.5
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailableNoInstall(xtl)
set(EXTERNAL_LIBS ${EXTERNAL_LIBS} xtl)

# Download and configure xsimd
message("${Yellow}Configuring XSIMD${ColorReset}")
FetchContent_Declare(xsimd
    GIT_REPOSITORY https://github.com/xtensor-stack/xsimd.git
    GIT_TAG 13.0.0
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailableNoInstall(xsimd)
set(EXTERNAL_LIBS ${EXTERNAL_LIBS} xsimd)

add_definitions("-DXTENSOR_USE_XSIMD=1")

# Download and configure xtensor
message("${Yellow}Configuring XTensor${ColorReset}")
set(XTENSOR_USE_XSIMD ON CACHE BOOL "")
FetchContent_Declare(xtensor
    GIT_REPOSITORY https://github.com/xtensor-stack/xtensor.git
    GIT_TAG 0.25.0
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailableNoInstall(xtensor)
set(EXTERNAL_LIBS ${EXTERNAL_LIBS} xtensor)

message("${Yellow}Configuring httplib${ColorReset}")
set(HTTPLIB_COMPILE ON CACHE BOOL "")
FetchContent_Declare(httplib 
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib
    GIT_TAG v0.14.1
)
FetchContent_MakeAvailableNoInstall(httplib)
if (NOT WIN32)
    target_compile_options(httplib PRIVATE -fPIC -DCPPHTTPLIB_KEEPALIVE_MAX_COUNT=5 -DCPPHTTPLIB_CONNECTION_TIMEOUT_SECOND=600 -DCPPHTTPLIB_READ_TIMEOUT_SECOND=120 -DCPPHTTPLIB_WRITE_TIMEOUT_SECOND=120 -DCPPHTTPLIB_IDLE_INTERVAL_USECOND=10000 -DCPPHTTPLIB_REQUEST_URI_MAX_LENGTH=8192 -DCPPHTTPLIB_HEADER_MAX_LENGTH=8192 -DCPPHTTPLIB_REDIRECT_MAX_COUNT=20)
else()
    target_compile_options(httplib PRIVATE -DCPPHTTPLIB_KEEPALIVE_MAX_COUNT=5 -DCPPHTTPLIB_CONNECTION_TIMEOUT_SECOND=600 -DCPPHTTPLIB_READ_TIMEOUT_SECOND=120 -DCPPHTTPLIB_WRITE_TIMEOUT_SECOND=120 -DCPPHTTPLIB_IDLE_INTERVAL_USECOND=10000 -DCPPHTTPLIB_REQUEST_URI_MAX_LENGTH=8192 -DCPPHTTPLIB_HEADER_MAX_LENGTH=8192 -DCPPHTTPLIB_REDIRECT_MAX_COUNT=20)
endif()

get_target_property(HTTPLIB_INCLUDE_DIRS httplib::httplib INTERFACE_INCLUDE_DIRECTORIES)

set(EXTERNAL_LIBS ${EXTERNAL_LIBS} httplib::httplib)

message("${Yellow}Configuring TracyProfiler${ColorReset}")
set(TRACY_STATIC ON CACHE BOOL "")
FetchContent_Declare(tracy 
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    GIT_REPOSITORY https://github.com/wolfpld/tracy
    GIT_TAG v0.10
)
FetchContent_MakeAvailableNoInstall(tracy)
if (NOT WIN32)
    target_compile_options(TracyClient PRIVATE -fPIC -DTRACY_EXPORTS)
else()
    target_compile_options(TracyClient PRIVATE -DTRACY_EXPORTS )
endif()

message(STATUS "Tracy include directories: ${tracy_SOURCE_DIR}/public")

if (WITH_SECURT_REST_CLIENT)
# Configure cpprestsdk
message("${Yellow}Configuring cpprestsdk${ColorReset}")
find_package(cpprestsdk REQUIRED)

# Configure Boost
message("${Yellow}Configuring Boost${ColorReset}")
find_package(Boost REQUIRED)
endif()

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX ${CVEDIA_INSTALL_DIR})
endif()

option(WITH_ONNX_MODULE "Build with ONNX module" OFF)
option(WITH_POST_CLASSIFIER_MODULE "Build with post-classifier module" ON)
option(WITH_SECURT_LIB "Build with SecuRT library" ON)
option(WITH_SECURT_LIB_PYTHON "Build with SecuRT python bindings" OFF)

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
#set(CMAKE_CXX_STANDARD 141)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
        if(ENABLE_CODE_COVERAGE)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
            SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 --coverage")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat-pedantic -Wno-conversion -Wno-exit-time-destructors -Wno-documentation")
        endif()
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-all -Wstack-protector --param ssp-buffer-size=4 -Wformat-security -Wextra -Wno-missing-braces -ftemplate-depth=2000")

    # setting warning flags here because otherwise they end up at the END of the command line
    # which makes it impossible to disable individual warnings with -Wno-* in other cmake files
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # enable colored outputs
        add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        # enable colored outputs
        add_compile_options (-fcolor-diagnostics)
    endif ()

    if(ENABLE_CODE_COVERAGE)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 --coverage")
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

include_directories(
    ${CMAKE_SOURCE_DIR}/include/sdk
    ${CMAKE_SOURCE_DIR}/RTCORE/include
    ${CMAKE_SOURCE_DIR}/3rdparty/libs/
    ${CMAKE_SOURCE_DIR}/3rdparty/
)

# Include sub-projects.
add_subdirectory ("samples/cli")
add_subdirectory ("samples/Plugins")
add_subdirectory ("src/")

install(DIRECTORY python DESTINATION ${CMAKE_INSTALL_PREFIX})

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
