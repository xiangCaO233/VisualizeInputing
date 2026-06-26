#pragma once

namespace vi
{

class NativeWindow;

/// @brief Dear ImGui 上下文和 GLFW/OpenGL3 backend 生命周期封装。
class ImGuiLayer
{
public:
    /// @brief 创建 ImGui 上下文并初始化 GLFW/OpenGL3 backend。
    /// @param window 已创建 OpenGL 上下文的原生窗口。
    explicit ImGuiLayer(NativeWindow& window);

    /// @brief 关闭 ImGui backend 并销毁 ImGui 上下文。
    ~ImGuiLayer();

    ImGuiLayer(ImGuiLayer&&)                 = delete;
    ImGuiLayer(const ImGuiLayer&)            = delete;
    ImGuiLayer& operator=(ImGuiLayer&&)      = delete;
    ImGuiLayer& operator=(const ImGuiLayer&) = delete;

    /// @brief 开始新一帧 ImGui。
    /// @warning UI 热路径：每帧调用，只能执行 ImGui backend 的 NewFrame。
    void beginFrame();

    /// @brief 提交当前帧 ImGui 绘制数据。
    /// @warning 渲染热路径：每帧调用，只提交 ImGui draw data。
    void renderDrawData();

private:
    /// @brief 获取当前平台应使用的 GLSL 版本字符串。
    /// @return GLSL `#version` 字符串。
    [[nodiscard]] const char* glslVersion() const;

    /// @brief 应用项目默认 ImGui 样式。
    void applyStyle();

    /// @brief 是否已经完成初始化。
    bool m_initialized{ false };
};

}  // namespace vi
