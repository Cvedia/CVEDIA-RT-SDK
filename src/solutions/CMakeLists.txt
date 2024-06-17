message("${Blue}Configuring SecuRT C API${ColorReset}")

if(WITH_SECURT_REST_CLIENT)
file(GLOB_RECURSE REST_SRCS "src/rest/*.cpp")
endif()

# Add source to this project's executable.
add_library(securt SHARED ${REST_SRCS} "src/securt/securt_capi.cpp" "src/common/functions.cpp"  "src/common/context.cpp" "src/core/core_capi.cpp" "src/core/core_sse_api.cpp")

set_target_properties(securt 
                        PROPERTIES 
                        PREFIX ""
                        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<0:>"
)

# Add the include directory for securt
if(WITH_SECURT_REST_CLIENT)
target_include_directories(securt
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rest/include ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../../include/sdk/
) 
else()
target_include_directories(securt
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../../include/sdk/
    $<TARGET_PROPERTY:TracyClient,INCLUDE_DIRECTORIES>
)
endif()

# set _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS 
if (WIN32)
	target_compile_definitions(securt PRIVATE _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS)
endif()

# Link securt with RTCORE
if(WITH_SECURT_REST_CLIENT)
if (WIN32)
    target_link_libraries(securt PUBLIC nlohmann_json plog Boost::headers cpprestsdk::cpprest bcrypt httplib::httplib ${RTCORELib})
else()
    target_link_libraries(securt PUBLIC nlohmann_json plog Boost::headers cpprestsdk::cpprest crypto httplib::httplib ${RTCORELib})
endif()
else()
if (WIN32)
    target_link_libraries(securt PUBLIC nlohmann_json plog httplib::httplib ${RTCORELib})
else()
    target_link_libraries(securt PUBLIC nlohmann_json plog httplib::httplib ${RTCORELib})
endif()
endif()

install(TARGETS securt DESTINATION ${CMAKE_INSTALL_PREFIX})
