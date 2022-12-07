# CMakeList.txt : CMake project for SynCore, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)
message("Configuring ImGui")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(imgui STATIC "ImGuiFileDialog/ImGuiFileDialog.cpp" "ImGuiColorTextEdit/TextEditor.cpp" "ImGuiMarkdown/imgui_md.cpp" 
        "ImGuiMarkdown/md4c.c" "imgui.cpp" "imgui_draw.cpp" "imgui_tables.cpp" "imgui_widgets.cpp" 
        "imgui_demo.cpp" "ImPlot/implot.cpp" "ImPlot/implot_items.cpp")

# Disable all warnings
target_compile_options(imgui PRIVATE -w)

target_include_directories(imgui
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> ${CMAKE_CURRENT_SOURCE_DIR} "" "backends/" "ImGuiFileDialog/" "imgui-notify/"  "examples/libs/glfw/include/" ${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} "../RTCORE/include/"
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
