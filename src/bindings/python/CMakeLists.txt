message("${Blue}Configuring SecuRT Python API${ColorReset}")

if (WIN32)
    # RT-180: Hacks for finding Python3 on Windows with VCPKG in the middle
    # Assumes PYTHON3_DIR has a version of python with requirements.txt installed.
    if (NOT DEFINED PYTHON3_DIR)
        set(PYTHON3_DIR "C:/Python312")
    endif()
    
    message(STATUS "PYTHON3_DIR: ${PYTHON3_DIR}")

    set(Python3_ROOT_DIR ${PYTHON3_DIR})
    
    set(Python3_FIND_STRATEGY LOCATION)
    set(Python3_EXECUTABLE ${Python3_ROOT_DIR}/python)
    set(PYTHONPATH ${Python3_EXECUTABLE})
    set(Python3_LIBRARY_DIRS ${Python3_ROOT_DIR}/libs ${Python3_ROOT_DIR}/Lib)
    set(Python3_INCLUDE_DIR ${Python3_ROOT_DIR}/include)
    set(Python3_NumPy_INCLUDE_DIR ${Python3_ROOT_DIR}/Lib/site-packages/numpy/core/include)
    
    message(STATUS "Python3_LIBRARY_DIRS: ${Python3_LIBRARY_DIRS}")
endif()

find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)

# SWIG setup
find_package(SWIG 4.0 REQUIRED)
include(${SWIG_USE_FILE})
set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -doxygen)

set_source_files_properties(SOURCE "cvediart.i" PROPERTY CPLUSPLUS OFF)

set (python_wrapper_name cvediartpy)

swig_add_library(${python_wrapper_name}
                 TYPE SHARED
                 LANGUAGE python
                 SOURCES "cvediart.i"
)

set (python_wrapper_target ${SWIG_MODULE_${python_wrapper_name}_REAL_NAME})

# Add the include directory for securt_swig
target_include_directories(${python_wrapper_target} 
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (NOT DEFINED PYTHON_BINDINGS_INSTALL_PATH)
    set(PYTHON_BINDINGS_INSTALL_PATH ${Python3_SITEARCH})
endif()

message(STATUS "PYTHON_BINDINGS_INSTALL_PATH: ${PYTHON_BINDINGS_INSTALL_PATH}")

# Link the SWIG wrapper with the existing source files
set_target_properties(${python_wrapper_target} PROPERTIES SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE)

# Now link the securt_swig target with the securt target
target_link_libraries(${python_wrapper_target} PRIVATE securt ${RTCORELib} Python3::Python Python3::NumPy)

set_target_properties(${python_wrapper_target} PROPERTIES COMPILE_WARNING_AS_ERROR FALSE)

install(TARGETS ${python_wrapper_target} DESTINATION ${PYTHON_BINDINGS_INSTALL_PATH}/cvedia/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/rt.py DESTINATION ${PYTHON_BINDINGS_INSTALL_PATH}/cvedia/)
file(WRITE "${CMAKE_INSTALL_PREFIX}/python/scripts/bindings/__init__.py" "")
