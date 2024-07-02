message("${Blue}Configuring SecuRT Python API${ColorReset}")

# TODO: Figure out why this doesn't work on CI, it finds vcpkg python 3.11
# but when run within local _build.bat it works with native python3 instead
# disabled WITH_SECURT_LIB_PYTHON for the timebeing.

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

# Link the SWIG wrapper with the existing source files
set_target_properties(${python_wrapper_target} PROPERTIES SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE)

# Now link the securt_swig target with the securt target
target_link_libraries(${python_wrapper_target} PRIVATE securt ${RTCORELib} Python3::Python Python3::NumPy)

set_target_properties(${python_wrapper_target} PROPERTIES COMPILE_WARNING_AS_ERROR FALSE)

install(TARGETS ${python_wrapper_target} DESTINATION ${Python3_SITEARCH}/cvedia/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/rt.py DESTINATION ${Python3_SITEARCH}/cvedia/)
file(WRITE "${CMAKE_INSTALL_PREFIX}/python/scripts/bindings/__init__.py" "")
