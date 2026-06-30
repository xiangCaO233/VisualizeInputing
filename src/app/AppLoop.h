#pragma once

namespace vi
{

class ImGuiLayer;
class GlobalInputListener;
class InputState;
class MainUi;
class NativeWindow;

/// @brief 应用主循环，负责按帧组织事件、UI 更新和 OpenGL 呈现。
class AppLoop
{
public:
    /// @brief 构造主循环。
    /// @param window 主原生窗口。
    /// @param imguiLayer ImGui 生命周期和 backend 层。
    /// @param mainUi 主 UI。
    /// @param inputState 输入状态。
    /// @param globalInputListener 全局输入监听器。
    AppLoop(NativeWindow& window, ImGuiLayer& imguiLayer, MainUi& mainUi,
            InputState& inputState, GlobalInputListener& globalInputListener);

    /// @brief 进入主循环直到窗口关闭。
    /// @return 退出码，0 表示正常退出。
    int run();

private:
    /// @brief 渲染当前帧到 OpenGL framebuffer。
    /// @warning 渲染热路径：每帧调用，只执行 framebuffer 查询、清屏和 ImGui
    /// 绘制提交。
    void renderFrame();

    /// @brief 主原生窗口观察引用。
    NativeWindow& m_window;

    /// @brief ImGui backend 层观察引用。
    ImGuiLayer& m_imguiLayer;

    /// @brief 主 UI 观察引用。
    MainUi& m_mainUi;

    /// @brief 输入状态观察引用。
    InputState& m_inputState;

    /// @brief 全局输入监听器观察引用。
    GlobalInputListener& m_globalInputListener;
};

}  // namespace vi
