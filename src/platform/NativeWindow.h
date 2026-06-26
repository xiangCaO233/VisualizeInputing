#pragma once

#include <string>

struct GLFWwindow;

namespace vi
{

/// @brief 原生 GLFW 窗口配置。
struct NativeWindowConfig {
    /// @brief 窗口标题。
    std::string title{ "VisualizeInputing" };

    /// @brief 初始窗口宽度。
    int width{ 1280 };

    /// @brief 初始窗口高度。
    int height{ 800 };

    /// @brief 是否启用垂直同步。
    bool enableVsync{ true };
};

/// @brief GLFW 原生窗口和 OpenGL 上下文封装。
class NativeWindow
{
public:
    /// @brief 创建 GLFW 窗口并初始化当前 OpenGL 上下文。
    /// @param config 窗口配置。
    explicit NativeWindow(NativeWindowConfig config);

    /// @brief 销毁 GLFW 窗口并终止 GLFW。
    ~NativeWindow();

    NativeWindow(NativeWindow&&)                 = delete;
    NativeWindow(const NativeWindow&)            = delete;
    NativeWindow& operator=(NativeWindow&&)      = delete;
    NativeWindow& operator=(const NativeWindow&) = delete;

    /// @brief 判断窗口是否收到关闭请求。
    /// @return 收到关闭请求时返回 true。
    [[nodiscard]] bool shouldClose() const;

    /// @brief 请求关闭窗口。
    void requestClose();

    /// @brief 轮询 GLFW 事件。
    void pollEvents() const;

    /// @brief 判断窗口是否处于最小化状态。
    /// @return 窗口最小化时返回 true。
    [[nodiscard]] bool isIconified() const;

    /// @brief 交换前后缓冲。
    void swapBuffers() const;

    /// @brief 设置垂直同步状态。
    /// @param enabled true 表示启用 VSync。
    void setVsync(bool enabled) const;

    /// @brief 获取 framebuffer 尺寸。
    /// @param width 输出 framebuffer 宽度。
    /// @param height 输出 framebuffer 高度。
    void getFramebufferSize(int& width, int& height) const;

    /// @brief 获取底层 GLFW 窗口句柄。
    /// @return GLFW 窗口句柄。
    [[nodiscard]] GLFWwindow* handle() const;

    /// @brief 获取当前 OpenGL 版本字符串。
    /// @return OpenGL 版本字符串，获取失败时返回 "unknown"。
    [[nodiscard]] const char* openGlVersion() const;

private:
    /// @brief 初始化 GLFW、创建窗口并绑定 OpenGL 上下文。
    void initialize();

    /// @brief 释放已经创建的 GLFW 资源。
    void shutdown();

    /// @brief 设置平台对应的 OpenGL window hints。
    void setupOpenGLHints() const;

    /// @brief 检查当前线程上的 OpenGL 上下文是否可用。
    void validateOpenGLContext() const;

    /// @brief 窗口配置。
    NativeWindowConfig m_config;

    /// @brief GLFW 窗口句柄。
    GLFWwindow* m_window{ nullptr };

    /// @brief GLFW 是否已初始化。
    bool m_glfwInitialized{ false };
};

}  // namespace vi
