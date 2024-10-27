cmake_minimum_required(VERSION 3.28)

project(visualizeinput)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_library(
	InputLsnr REQUIRED
	NAMES InputLsnr-static
	PATHS "${CMAKE_SOURCE_DIR}/../InputListener/build")

include_directories(include)
set(SOURCES src/main.cpp src/gls/shader.cpp src/gls/texture.cpp
            src/gls/glfws/windowcore.cpp)

add_executable(visualizeinput ${SOURCES})

target_link_libraries(visualizeinput glfw GLEW::GLEW OpenGL::GL 
	${InputLsr}
)
