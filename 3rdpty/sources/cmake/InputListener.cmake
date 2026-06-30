# InputListener 源码包装。
#
# 业务 target 只链接 3rd_inputlistener；示例程序在这里关闭，第三方源码保持不变。
set(INPUTLISTENER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/InputListener")

if(NOT EXISTS "${INPUTLISTENER_SOURCE_DIR}/CMakeLists.txt")
  message(
    FATAL_ERROR
      "InputListener submodule is missing: ${INPUTLISTENER_SOURCE_DIR}")
endif()

set(INPUTLISTENER_BUILD_EXAMPLES
    OFF
    CACHE BOOL "Disable InputListener examples" FORCE)

add_subdirectory(
  "${INPUTLISTENER_SOURCE_DIR}"
  "${CMAKE_CURRENT_BINARY_DIR}/InputListener-build" EXCLUDE_FROM_ALL SYSTEM)

add_library(3rd_inputlistener INTERFACE)
target_link_libraries(3rd_inputlistener INTERFACE InputListener::InputListener)
