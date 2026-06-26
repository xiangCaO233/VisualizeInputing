# Dear ImGui 源码包装。
#
# 本项目使用 GLFW 平台后端和 OpenGL3 渲染后端。业务 target 只链接 3rd_imgui；ImGui 源码和 backend
# 文件都在这里收口，避免顶层 CMake 直接感知 第三方仓库结构。
set(IMGUI_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui")

if(NOT EXISTS "${IMGUI_SOURCE_DIR}/imgui.cpp")
  message(FATAL_ERROR "Dear ImGui submodule is missing: ${IMGUI_SOURCE_DIR}")
endif()

add_library(
  imgui STATIC
  "${IMGUI_SOURCE_DIR}/imgui.cpp"
  "${IMGUI_SOURCE_DIR}/imgui_demo.cpp"
  "${IMGUI_SOURCE_DIR}/imgui_draw.cpp"
  "${IMGUI_SOURCE_DIR}/imgui_tables.cpp"
  "${IMGUI_SOURCE_DIR}/imgui_widgets.cpp"
  "${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
  "${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp")

target_include_directories(imgui SYSTEM PUBLIC "${IMGUI_SOURCE_DIR}"
                                               "${IMGUI_SOURCE_DIR}/backends")

target_link_libraries(imgui PUBLIC 3rd_glfw)

add_library(3rd_imgui INTERFACE)
target_link_libraries(3rd_imgui INTERFACE imgui)
