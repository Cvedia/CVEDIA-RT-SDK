cmake_minimum_required(VERSION 3.15)

add_executable( runinference "runinference.cpp")

target_include_directories(runinference
    PUBLIC "../../../RTCORE/include/"
)

target_link_libraries(runinference RTCORE ${OPENCV_LIBRARIES})

install(TARGETS runinference DESTINATION ${CMAKE_INSTALL_PREFIX})
