# ImPlot 源码包装。
#
# ImPlot 依赖 Dear ImGui context 和绘制接口。业务 target 只链接 3rd_implot； 第三方源码文件在本包装内收口。
set(IMPLOT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/implot")

if(NOT EXISTS "${IMPLOT_SOURCE_DIR}/implot.cpp")
  message(FATAL_ERROR "ImPlot submodule is missing: ${IMPLOT_SOURCE_DIR}")
endif()

add_library(implot STATIC "${IMPLOT_SOURCE_DIR}/implot.cpp"
                          "${IMPLOT_SOURCE_DIR}/implot_items.cpp")

target_include_directories(implot SYSTEM PUBLIC "${IMPLOT_SOURCE_DIR}")

target_link_libraries(implot PUBLIC 3rd_imgui)

add_library(3rd_implot INTERFACE)
target_link_libraries(3rd_implot INTERFACE implot)
