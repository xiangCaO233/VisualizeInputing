# VisualizeInputing 智能体指令

本文档面向在 `VisualizeInputing` 仓库中工作的 AI 编程智能体。修改时应优先参考本项目现有代码，同时沿用 `MusicMapMaker-Next` 的最小化修改、先理解再修改、第三方边界清晰等原则。

## 0. 核心原则

- **最小化修改**: 只处理用户明确提出的需求，不做未授权的大规模重构。
- **先理解再修改**: 修改前优先使用 `rg`、`rg --files` 等工具了解相关代码和项目结构。
- **命令执行位置**: 运行 shell 命令时必须在项目根目录执行。
- **文件操作路径**: 文件读写、格式化等操作优先使用绝对路径。
- **第三方代码边界**: 不要修改 `3rdpty/sources/` 下的第三方源码；需要调整构建行为时，修改本仓库维护的 CMake 包装文件。
- **依赖 API 防过时**: 对 Dear ImGui、GLFW、OpenGL、InputListener 等第三方库编写新调用代码前，优先确认本仓库已锁定版本的头文件和官方文档。

## 1. 项目概览

- **语言**: C++17。
- **构建系统**: CMake，最低版本 3.28。
- **UI 框架**: Dear ImGui docking 分支。
- **图形栈**: OpenGL、GLFW，ImGui 使用 GLFW 平台后端和 OpenGL3 渲染后端。
- **输入监听**: 当前应用先使用 ImGui/GLFW 输入流展示窗口内输入；`InputListener` 子模块保留给后续全局输入监听能力。
- **第三方依赖目录**: `3rdpty/sources/`。
- **资源目录**: `assets/`，当前保留字体和历史 shader 资源。

## 2. 目录边界

- **`src/app/`**: 当前应用层。`Application` 负责 GLFW 窗口、OpenGL 上下文、ImGui 生命周期和主循环。
- **`src/main.cpp`**: 只负责创建并运行 `vi::Application`，不要在这里堆业务逻辑。
- **`include/`**: 项目内置头文件，目前包含 `stb_truetype.h`。
- **`assets/`**: 运行时资源，新增或移动资源时必须同步检查代码中的相对路径。
- **`3rdpty/`**: 第三方依赖和本仓库维护的依赖包装。
  - 允许修改 `3rdpty/CMakeLists.txt`、`3rdpty/sources/CMakeLists.txt`、`3rdpty/sources/cmake/*.cmake`。
  - 禁止修改 `3rdpty/sources/glfw`、`3rdpty/sources/imgui`、`3rdpty/sources/InputListener` 内的第三方源码。

## 3. 构建与格式化

### 3.1 构建

```bash
git submodule update --init --recursive
cmake -S . -B build -G Ninja
cmake --build build
```

未使用 Ninja 时，可省略 `-G Ninja` 使用本机默认生成器。

### 3.2 依赖接入

- 可源码化依赖必须作为 Git submodule 放在 `3rdpty/sources/<name>`。
- 顶层 `CMakeLists.txt` 只通过 `add_subdirectory("3rdpty")` 进入依赖入口。
- `3rdpty/sources/CMakeLists.txt` 只包含本仓库维护的包装脚本。
- 每个依赖使用独立包装脚本，例如 `3rdpty/sources/cmake/GLFW.cmake`。
- 业务 target 只链接包装 target，例如 `3rd_glfw`、`3rd_imgui`。
- `OpenGL` 是系统图形 API，仍通过 `find_package(OpenGL REQUIRED)` 获取。
- `InputListener` 当前不参与主程序链接；后续接入全局输入监听时再新增独立包装入口和应用层抽象。

### 3.3 修改后的验证

- 修改 CMake 或源码后，必须至少运行：

```bash
cmake -S . -B build
cmake --build build
```

- 如果只修改文档或配置且不影响构建，可说明未运行构建的原因。

### 3.4 格式化

- 修改过的 `.cpp`、`.h`、`.hpp` 文件，完成前必须运行：

```bash
clang-format -i <absolute_file_path>
```

- 修改过的本仓库自维护 CMake 脚本，完成前必须运行：

```bash
cmake-format -i <absolute_file_path>
```

- CMake 格式化范围包括：
  - `CMakeLists.txt`
  - `3rdpty/CMakeLists.txt`
  - `3rdpty/sources/CMakeLists.txt`
  - `3rdpty/sources/cmake/*.cmake`
- 禁止格式化或修改 `3rdpty/sources/` 下第三方仓库自己的 CMake 文件。

## 4. C++ 代码规范

- **风格来源**: 遵循仓库根目录 `.clang-format`。
- **新增代码命名**: 优先贴近周围代码，不为统一命名而重写无关文件。
- **注释语言**: 新增说明性注释优先使用中文；API、类型名、宏名和协议标识可以保留原文。
- **头文件保护**: 新增头文件使用 `#pragma once`。
- **Includes 顺序**: 对应头文件优先，其次项目头文件，然后第三方头文件，最后标准库头文件。
- **日志与调试输出**: 避免留下临时 `std::cout`、`printf` 调试输出；当前项目没有统一日志封装时，必要输出应保持局部且可解释。
- **窗口与渲染循环**: 当前设计是主线程持有 GLFW/OpenGL/ImGui 生命周期，不要重新引入 detach 渲染线程或平台分支窗口类。
- **异常与资源**: 修改窗口、OpenGL 上下文、ImGui 初始化相关逻辑时，必须保证构造失败会清理已经创建的资源。

## 5. Git 与第三方源码

- **提交范围**: 不把构建产物、缓存目录、IDE 临时文件加入版本控制。
- **子模块更新**: 新增或更新依赖时，同步检查 `.gitmodules` 和 `git submodule status`。
- **第三方补丁**: 不在第三方子模块内直接打补丁；确有必要时先向用户说明原因，再决定 fork、固定 commit 或在包装层规避。
