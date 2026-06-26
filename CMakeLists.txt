cmake_minimum_required(VERSION 3.28)

project(visualizeinput LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

add_subdirectory("3rdpty")

set(SOURCES
    src/main.cpp
    src/app/Application.cpp
    src/app/AppLoop.cpp
    src/imgui/ImGuiLayer.cpp
    src/input/InputState.cpp
    src/platform/NativeWindow.cpp
    src/ui/MainUi.cpp)

add_executable(visualizeinput ${SOURCES})

target_include_directories(visualizeinput
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(visualizeinput PRIVATE 3rd_glfw 3rd_imgui OpenGL::GL)
