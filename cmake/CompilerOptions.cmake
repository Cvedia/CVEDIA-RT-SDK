# CompilerOptions.cmake

if(NOT DEFINED CXX_VERSION)
    message(STATUS "CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")
    message(STATUS "CMAKE_CXX_COMPILER_VERSION = ${CMAKE_CXX_COMPILER_VERSION}")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8)
            message(STATUS "GCC version < 8, C++17 is not supported, using C++14.")
            set(CXX_VERSION 14)
        else()
            message(STATUS "GCC version >= 8, using C++17.")
            set(CXX_VERSION 17)
        endif()
    else()
        message(STATUS "Non-GCC compiler, using C++17.")
        set(CXX_VERSION 17)
    endif()
endif()

set(CMAKE_CXX_STANDARD ${CXX_VERSION})
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_COMPILE_WARNING_AS_ERROR TRUE)

include ("cmake/CheckMutex.cmake")

if (NOT MSVC)
    if(ENABLE_CODE_COVERAGE)
        add_compile_options(-g -O0 --coverage -fprofile-arcs -ftest-coverage)
    endif()
endif()

