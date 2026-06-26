# 第三方依赖入口。
#
# 本项目的可源码化依赖均以 Git submodule 放在 3rdpty/sources 下。顶层 CMake
# 只进入本目录，不直接感知各第三方库的源码结构。
add_subdirectory(sources)
