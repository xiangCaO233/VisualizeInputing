cmake_minimum_required(VERSION 3.28)

project(VisualizeInputing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(APPLE)
  # 添加库文件路径
  link_directories("../InputListener")
elseif(UNIX)

endif()

# 查找动态库 libInputLsnr-dynamic.dylib
find_library(
  INPUT_LSNR_LIB
  NAMES InputLsnr-dynamic
  PATHS ../InputListener)

set(INCLUDES src/headers ../InputListener/src/headers)

set(SOURCES
    src/cpp/main.cpp src/cpp/lsr/GlobalKeyListener.cpp
    src/cpp/lsr/GlobalMouseListener.cpp src/cpp/KeyWidget.cpp
    src/cpp/MouseWidget.cpp src/cpp/Caller.cpp)

find_package(
  Qt6
  COMPONENTS Core Gui Widgets
  REQUIRED)

add_executable(VisualizeInputing ${SOURCES})

target_include_directories(VisualizeInputing PUBLIC ${INCLUDES})
target_link_libraries(VisualizeInputing Qt::Core Qt::Gui Qt::Widgets
                      ${INPUT_LSNR_LIB})
