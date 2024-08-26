message("${Blue}Configuring SecuRT .NET API${ColorReset}")

# SWIG setup
find_package(SWIG 4.0 REQUIRED)
include(${SWIG_USE_FILE})
set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -namespace Cvedia)

set(SWIG_FILES core_csharp.i securt_csharp.i system_csharp.i rt_csharp.i solutions_csharp.i license_csharp.i)

foreach(FILE ${SWIG_FILES})
    set_source_files_properties(SOURCE ${FILE} PROPERTY CPLUSPLUS OFF)
endforeach()

set (csharp_wrapper_name securtcsharp)

swig_add_library(${csharp_wrapper_name}
                 TYPE SHARED
                 LANGUAGE csharp
                 SOURCES ${SWIG_FILES}
)

set (csharp_wrapper_target ${SWIG_MODULE_${csharp_wrapper_name}_REAL_NAME})

# Add the include directory for securt_swig
target_include_directories(${csharp_wrapper_target} 
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link the SWIG wrapper with the existing source files
set_target_properties(${csharp_wrapper_target} PROPERTIES SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE)

# Now link the securt_swig target with the securt target
target_link_libraries(${csharp_wrapper_target} PRIVATE securt ${RTCORELib})

set_target_properties(${csharp_wrapper_target} PROPERTIES COMPILE_WARNING_AS_ERROR FALSE)

install(TARGETS ${csharp_wrapper_target} DESTINATION ${CMAKE_INSTALL_PREFIX}/dotnet/scripts/bindings/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Core.cs DESTINATION ${CMAKE_INSTALL_PREFIX}/dotnet/scripts/bindings/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Rt.cs DESTINATION ${CMAKE_INSTALL_PREFIX}/dotnet/scripts/bindings/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Securt.cs DESTINATION ${CMAKE_INSTALL_PREFIX}/dotnet/scripts/bindings/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/System.cs DESTINATION ${CMAKE_INSTALL_PREFIX}/dotnet/scripts/bindings/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Solutions.cs DESTINATION ${CMAKE_INSTALL_PREFIX}/dotnet/scripts/bindings/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/License.cs DESTINATION ${CMAKE_INSTALL_PREFIX}/dotnet/scripts/bindings/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CorePINVOKE.cs DESTINATION ${CMAKE_INSTALL_PREFIX}/dotnet/scripts/bindings/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RtPINVOKE.cs DESTINATION ${CMAKE_INSTALL_PREFIX}/dotnet/scripts/bindings/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SecurtPINVOKE.cs DESTINATION ${CMAKE_INSTALL_PREFIX}/dotnet/scripts/bindings/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SystemPINVOKE.cs DESTINATION ${CMAKE_INSTALL_PREFIX}/dotnet/scripts/bindings/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SolutionsPINVOKE.cs DESTINATION ${CMAKE_INSTALL_PREFIX}/dotnet/scripts/bindings/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LicensePINVOKE.cs DESTINATION ${CMAKE_INSTALL_PREFIX}/dotnet/scripts/bindings/)
