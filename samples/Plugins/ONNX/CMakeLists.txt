message("${Blue}Configuring ONNX plugin${ColorReset}")

if(WIN32)
    if(EXISTS ${ONNX_PATH})
        set(ONNX_DIR ${ONNX_PATH})
    else()
        message( FATAL_ERROR "ONNX_DIR Environment Variable (${ONNX_PATH}) not set or points to invalid directory. Check CMakePaths.txt" )
    endif()    
else()
    if (NOT DEFINED ONNX_DIR)
        if(EXISTS "/opt/onnxruntime/")
            set(ONNX_DIR "/opt/onnxruntime/")
        else()
            message( FATAL_ERROR "onnxruntime not found! must be present at /opt/onnxruntime/" )
        endif()
    else()
        message(STATUS "Using predefined ONNX_DIR: ${ONNX_DIR}")
    endif()
endif()

if (LINK_ONNX_STATIC)
# Add source to this project's executable.
add_library (ONNX_static STATIC "src/onnxcore.cpp" "src/onnxinferencehandler.cpp")
set_target_properties(ONNX_static 
                        PROPERTIES 
                        PREFIX ""
                        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Plugins/$<0:>")

else()
add_library (ONNX MODULE "src/onnxcore.cpp" "src/onnxinferencehandler.cpp")
set_target_properties(ONNX 
                        PROPERTIES 
                        PREFIX ""
                        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Plugins/$<0:>")

endif()

message( STATUS "Configuring ONNX backends:")
if (UNIX)
    if (EXISTS ${ONNX_DIR}/build/Linux/Release/libonnxruntime_providers_cuda.so)
        message(STATUS "Onnx plugin: ONNX_HAS_CUDA")
        if (LINK_ONNX_STATIC)
            target_compile_definitions(ONNX_static PUBLIC "ONNX_HAS_CUDA")
        else()
            target_compile_definitions(ONNX PUBLIC "ONNX_HAS_CUDA")
        endif()
    endif()
    if (EXISTS ${ONNX_DIR}/build/Linux/Release/libonnxruntime_providers_tensorrt.so)
        message(STATUS "Onnx plugin: ONNX_HAS_TENSORRT")
        if (LINK_ONNX_STATIC)
            target_compile_definitions(ONNX_static PUBLIC "ONNX_HAS_TENSORRT")
        else()
            target_compile_definitions(ONNX PUBLIC "ONNX_HAS_TENSORRT")
        endif()
    endif()
else()
    message( STATUS "Onnx plugin: DML")
    message( STATUS "Onnx plugin: CUDA")
    message( STATUS "Onnx plugin: TRT")
    if (LINK_ONNX_STATIC)
        target_compile_definitions(ONNX_static PUBLIC "ONNX_HAS_DML")
        target_compile_definitions(ONNX_static PUBLIC "ONNX_HAS_CUDA")
        target_compile_definitions(ONNX_static PUBLIC "ONNX_HAS_TENSORRT")
    else()
        target_compile_definitions(ONNX PUBLIC "ONNX_HAS_DML")
        target_compile_definitions(ONNX PUBLIC "ONNX_HAS_CUDA")
        target_compile_definitions(ONNX PUBLIC "ONNX_HAS_TENSORRT")
    endif()
endif()

get_target_property(XTENSOR_INCLUDE_DIRS xtensor INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(XTL_INCLUDE_DIRS xtl INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(XSIMD_INCLUDE_DIRS xsimd INTERFACE_INCLUDE_DIRECTORIES)

if (LINK_ONNX_STATIC)
    target_include_directories(ONNX_static
        SYSTEM PRIVATE ${ONNX_DIR}/include/onnxruntime/core/session 
            ${ONNX_DIR}/onnxruntime/core/providers/cuda ${ONNX_DIR}/include/onnxruntime/core/providers/tensorrt 
            ${ONNX_DIR}/include/onnxruntime/core/providers/dml 
            ${OPENCV_INCLUDE_DIRS} ${ONNX_DIR}/include/onnxruntime/
            ${XTENSOR_INCLUDE_DIRS} ${XTL_INCLUDE_DIRS} ${XSIMD_INCLUDE_DIRS}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/sdk/
    )
else()
    target_include_directories(ONNX
        SYSTEM PRIVATE ${ONNX_DIR}/include/onnxruntime/core/session 
            ${ONNX_DIR}/onnxruntime/core/providers/cuda ${ONNX_DIR}/include/onnxruntime/core/providers/tensorrt 
            ${ONNX_DIR}/include/onnxruntime/core/providers/dml 
            ${OPENCV_INCLUDE_DIRS} ${ONNX_DIR}/include/onnxruntime/
            ${XTENSOR_INCLUDE_DIRS} ${XTL_INCLUDE_DIRS} ${XSIMD_INCLUDE_DIRS}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/sdk/
    )
endif()

if (LINK_ONNX_STATIC)
    if(WIN32)
        target_link_libraries(ONNX_static PRIVATE RTCORE_3rdparty plog ${ONNX_DIR}/build/Windows/RelWithDebInfo/RelWithDebInfo/onnxruntime.lib)
    else()
        # The Release is being kept on purpose
        target_link_libraries(ONNX_static PUBLIC RTCORE_3rdparty plog ${ONNX_DIR}/build/Linux/Release/libonnxruntime.so ${ONNX_DIR}/build/Linux/Release/libonnxruntime_providers_shared.so ${OPENCV_LIBRARIES})
    endif()
else()
    if(WIN32)
#        target_link_options(ONNX PRIVATE 
#            "/DELAYLOAD:onnxruntime.dll")

        target_link_libraries(ONNX PRIVATE ${RTCORELib} plog ${ONNX_DIR}/build/Windows/RelWithDebInfo/RelWithDebInfo/onnxruntime.lib)
    else()
        # The Release is being kept on purpose
        target_link_libraries(ONNX PUBLIC ${RTCORELib} plog ${ONNX_DIR}/build/Linux/Release/libonnxruntime.so ${ONNX_DIR}/build/Linux/Release/libonnxruntime_providers_shared.so ${OPENCV_LIBRARIES})
    endif()
    install(TARGETS ONNX DESTINATION ${CMAKE_INSTALL_PREFIX}/Plugins)
endif()

