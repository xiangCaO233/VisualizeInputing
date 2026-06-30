#pragma once

#include <array>
#include <deque>
#include <map>
#include <string>

namespace vi
{

class GlobalInputListener;
struct GlobalInputEvent;

/// @brief 全局输入状态和事件日志。
class InputState
{
public:
    /// @brief 从全局监听队列消费事件并更新当前输入状态。
    /// @param globalInput 全局输入监听器。
    /// @warning UI 热路径：每帧执行，只在主线程处理已入队事件。
    void consumeGlobalInput(GlobalInputListener& globalInput);

    /// @brief 清空事件日志。
    void clearEvents();

    /// @brief 获取最近输入事件。
    /// @return 最近事件序列，最新事件位于队首。
    [[nodiscard]] const std::deque<std::string>& events() const;

    /// @brief 获取当前处于按下状态的键盘按键。
    /// @return 原始键码到显示文本的映射。
    [[nodiscard]] const std::map<int, std::string>& activeKeys() const;

    /// @brief 判断指定原始键码是否处于按下状态。
    /// @param rawCode InputListener 平台原始键码。
    /// @return 按下时返回 true。
    [[nodiscard]] bool isKeyDown(int rawCode) const;

    /// @brief 判断指定鼠标按钮是否处于按下状态。
    /// @param button InputListener 鼠标按钮编号。
    /// @return 按下时返回 true。
    [[nodiscard]] bool isMouseButtonDown(int button) const;

    /// @brief 获取全局监听器累计的鼠标相对 x 坐标。
    /// @return 鼠标相对 x 坐标。
    [[nodiscard]] double mouseX() const;

    /// @brief 获取全局监听器累计的鼠标相对 y 坐标。
    /// @return 鼠标相对 y 坐标。
    [[nodiscard]] double mouseY() const;

    /// @brief 获取当前帧累计的水平滚动增量。
    /// @return 水平滚动增量。
    [[nodiscard]] double scrollX() const;

    /// @brief 获取当前帧累计的垂直滚动增量。
    /// @return 垂直滚动增量。
    [[nodiscard]] double scrollY() const;

private:
    /// @brief 应用一条全局输入事件。
    /// @param event 全局输入事件。
    void applyGlobalEvent(const GlobalInputEvent& event);

    /// @brief 追加一条事件日志，并裁剪到最大容量。
    /// @param eventText 事件文本。
    void pushEvent(std::string eventText);

    /// @brief 当前处于按下状态的键盘按键。
    std::map<int, std::string> m_activeKeys;

    /// @brief 当前处于按下状态的鼠标按钮。
    std::array<bool, 6> m_mouseButtons{};

    /// @brief 全局监听器累计的鼠标相对 x 坐标。
    double m_mouseX{ 0.0 };

    /// @brief 全局监听器累计的鼠标相对 y 坐标。
    double m_mouseY{ 0.0 };

    /// @brief 当前帧累计水平滚动增量。
    double m_scrollX{ 0.0 };

    /// @brief 当前帧累计垂直滚动增量。
    double m_scrollY{ 0.0 };

    /// @brief 最近输入事件。
    std::deque<std::string> m_events;
};

}  // namespace vi
