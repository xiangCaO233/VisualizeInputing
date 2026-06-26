#pragma once

#include <deque>
#include <string>

namespace vi
{

/// @brief 当前帧输入记录和事件日志。
class InputState
{
public:
    /// @brief 从 ImGui 输入状态记录本帧发生的按键和鼠标事件。
    /// @warning UI 热路径：每帧执行，只遍历 ImGui 已知键位和少量鼠标按键。
    void recordFrameInput();

    /// @brief 清空事件日志。
    void clearEvents();

    /// @brief 获取最近输入事件。
    /// @return 最近事件序列，最新事件位于队首。
    [[nodiscard]] const std::deque<std::string>& events() const;

private:
    /// @brief 追加一条事件日志，并裁剪到最大容量。
    /// @param eventText 事件文本。
    void pushEvent(std::string eventText);

    /// @brief 最近输入事件。
    std::deque<std::string> m_events;
};

}  // namespace vi
