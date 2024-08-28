add_executable( rtspdemo "rtspdemo.cpp")

target_include_directories(rtspdemo
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/sdk/
)

target_link_libraries(rtspdemo PRIVATE securt ${RTCORELib} plog nlohmann_json)

install(TARGETS rtspdemo DESTINATION ${CMAKE_INSTALL_PREFIX})
