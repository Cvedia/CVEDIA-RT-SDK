add_executable( pfdemo_c "pfdemo.c")

message("----------------------------")
message("${CMAKE_CURRENT_SOURCE_DIR}")
message("----------------------------")

target_include_directories(pfdemo_c
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/sdk/api
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/solutions/include/core
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/solutions/include/securt
)

target_link_libraries(pfdemo_c PRIVATE securt ${RTCORELib} )

install(TARGETS pfdemo_c DESTINATION ${CMAKE_INSTALL_PREFIX})
