#pragma once

#include <array>

namespace vi
{

class InputState;
class NativeWindow;

/// @brief 主 ImGui 界面，负责菜单和输入监听工作区绘制。
class MainUi
{
public:
    /// @brief 构造主 UI 状态。
    /// @param enableVsync 初始 VSync 状态。
    explicit MainUi(bool enableVsync);

    /// @brief 绘制一帧主 UI。
    /// @param window 主原生窗口。
    /// @param inputState 输入状态。
    /// @warning UI 热路径：每帧执行，只绘制 ImGui 控件和读取轻量状态。
    void render(NativeWindow& window, InputState& inputState);

    /// @brief 获取当前清屏颜色。
    /// @return RGBA 清屏颜色。
    [[nodiscard]] const std::array<float, 4>& clearColor() const;

private:
    /// @brief 绘制铺满原生窗口的主 ImGui 窗口。
    /// @param window 主原生窗口。
    /// @param inputState 输入状态。
    void renderMainWindow(NativeWindow& window, InputState& inputState);

    /// @brief 绘制主窗口内部菜单栏。
    /// @param window 主原生窗口。
    void renderMainMenu(NativeWindow& window);

    /// @brief 绘制监听组件工作区。
    /// @param window 主原生窗口。
    /// @param inputState 输入状态。
    void renderListenerWorkspace(NativeWindow& window, InputState& inputState);

    /// @brief 绘制输入状态、键鼠可视化和设置面板。
    /// @param window 主原生窗口。
    /// @param inputState 输入状态。
    void renderListenerPanels(NativeWindow&     window,
                              const InputState& inputState);

    /// @brief 绘制输入状态组件。
    /// @param inputState 输入状态。
    void renderInputMonitor(const InputState& inputState);

    /// @brief 绘制键鼠可视化组件。
    /// @param inputState 输入状态。
    void renderVisualizer(const InputState& inputState);

    /// @brief 绘制运行设置组件。
    /// @param window 主原生窗口。
    void renderSettings(NativeWindow& window);

    /// @brief 绘制输入事件日志组件。
    /// @param inputState 输入状态。
    void renderEventLog(InputState& inputState);

    /// @brief 绘制一个键位按钮。
    /// @param inputState 输入状态。
    /// @param label 显示标签。
    /// @param key InputListener 平台原始键码。
    /// @param width 按钮宽度。
    void drawKey(const InputState& inputState, const char* label, int key,
                 float width = 44.0f);

    /// @brief 绘制一个鼠标按钮。
    /// @param inputState 输入状态。
    /// @param label 显示标签。
    /// @param button InputListener 鼠标按钮编号。
    /// @param width 按钮宽度。
    void drawMouseButton(const InputState& inputState, const char* label,
                         int button, float width = 44.0f);

    /// @brief 绘制一个输入状态按钮。
    /// @param label 显示标签。
    /// @param isDown 是否处于按下状态。
    /// @param width 按钮宽度。
    void drawInputButton(const char* label, bool isDown, float width);

    /// @brief 绘制一行键位按钮。
    /// @param inputState 输入状态。
    /// @param labels 标签数组。
    /// @param keys InputListener 平台原始键码数组。
    /// @param count 键位数量。
    void drawKeyRow(const InputState& inputState, const char* const* labels,
                    const int* keys, int count);

    /// @brief 是否显示 ImGui demo 窗口。
    bool m_showDemoWindow{ false };

    /// @brief 是否显示 ImGui metrics 窗口。
    bool m_showMetricsWindow{ false };

    /// @brief 是否显示输入事件日志。
    bool m_showEventLog{ true };

    /// @brief 当前 VSync 设置。
    bool m_enableVsync{ true };

    /// @brief 当前清屏颜色。
    std::array<float, 4> m_clearColor{ 0.08f, 0.09f, 0.10f, 1.0f };
};

}  // namespace vi
