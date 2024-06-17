message("${Blue}Configuring postclassifier plugin${ColorReset}")

# Add source to this project's executable.
add_library (postclassifier MODULE "src/postclassifier.cpp")

get_target_property(XTENSOR_INCLUDE_DIRS xtensor INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(XTL_INCLUDE_DIRS xtl INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(XSIMD_INCLUDE_DIRS xsimd INTERFACE_INCLUDE_DIRECTORIES)

set_target_properties(postclassifier 
                        PROPERTIES 
                        PREFIX ""
                        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Plugins/$<0:>")

target_include_directories(postclassifier
    SYSTEM PRIVATE
        ${XTENSOR_INCLUDE_DIRS} ${XTL_INCLUDE_DIRS} ${XSIMD_INCLUDE_DIRS}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/sdk/
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        $<TARGET_PROPERTY:TracyClient,INCLUDE_DIRECTORIES>
)

target_link_libraries (postclassifier PUBLIC ${RTCORELib} plog)

install(TARGETS postclassifier DESTINATION ${CMAKE_INSTALL_PREFIX}/Plugins)
