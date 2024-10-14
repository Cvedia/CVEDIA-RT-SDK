add_executable( pfdemo_cpp "pfdemo.cpp")

target_include_directories(pfdemo_cpp
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/sdk/
)

target_link_libraries(pfdemo_cpp PRIVATE securt ${RTCORELib} plog nlohmann_json)

install(TARGETS pfdemo_cpp DESTINATION ${CMAKE_INSTALL_PREFIX})
