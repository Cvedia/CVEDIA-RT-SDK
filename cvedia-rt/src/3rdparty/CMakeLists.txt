if (BUILD_XTENSOR_LIB)
    message(STATUS "Building XTENSOR lib")
    find_package("OpenMP")
    add_subdirectory ("xsimd")

    add_subdirectory ("xtl")
    add_subdirectory ("xtensor")
endif()
