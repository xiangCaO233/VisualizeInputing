# GLFW 源码包装。
#
# 业务 target 只链接 3rd_glfw；GLFW 自身的示例、测试、文档和安装规则在这里 关闭，避免第三方子项目影响主项目的默认构建目标。
set(GLFW_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glfw")

if(NOT EXISTS "${GLFW_SOURCE_DIR}/CMakeLists.txt")
  message(FATAL_ERROR "GLFW submodule is missing: ${GLFW_SOURCE_DIR}")
endif()

set(GLFW_BUILD_DOCS
    OFF
    CACHE BOOL "Disable GLFW documentation" FORCE)
set(GLFW_BUILD_TESTS
    OFF
    CACHE BOOL "Disable GLFW tests" FORCE)
set(GLFW_BUILD_EXAMPLES
    OFF
    CACHE BOOL "Disable GLFW examples" FORCE)
set(GLFW_INSTALL
    OFF
    CACHE BOOL "Disable GLFW install target" FORCE)
set(GLFW_LIBRARY_TYPE
    STATIC
    CACHE STRING "Build GLFW as a static library" FORCE)

if(UNIX AND NOT APPLE)
  set(GLFW_BUILD_X11
      ON
      CACHE BOOL "Enable GLFW X11 backend" FORCE)
  set(GLFW_BUILD_WAYLAND
      OFF
      CACHE BOOL "Disable GLFW Wayland backend" FORCE)
endif()

add_subdirectory("${GLFW_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/glfw-build"
                 EXCLUDE_FROM_ALL SYSTEM)

add_library(3rd_glfw INTERFACE)
target_link_libraries(3rd_glfw INTERFACE glfw)
