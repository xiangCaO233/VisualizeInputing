# concurrentqueue 头文件库包装。
#
# 业务 target 只链接 3rd_concurrentqueue；这里不进入第三方 CMake，避免它的 示例、测试或安装规则影响主项目默认构建。
set(CONCURRENTQUEUE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/concurrentqueue")

if(NOT EXISTS "${CONCURRENTQUEUE_SOURCE_DIR}/concurrentqueue.h")
  message(
    FATAL_ERROR
      "concurrentqueue submodule is missing: ${CONCURRENTQUEUE_SOURCE_DIR}")
endif()

add_library(concurrentqueue INTERFACE)
target_include_directories(concurrentqueue SYSTEM
                           INTERFACE "${CONCURRENTQUEUE_SOURCE_DIR}")

add_library(3rd_concurrentqueue INTERFACE)
target_link_libraries(3rd_concurrentqueue INTERFACE concurrentqueue)
