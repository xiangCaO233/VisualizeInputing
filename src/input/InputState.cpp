#include "input/InputState.h"

#include <imgui.h>

#include <cstddef>
#include <utility>

namespace vi
{
namespace
{

/// @brief 输入事件日志最大条数。
constexpr std::size_t MAX_EVENT_COUNT = 64;

}  // namespace

void InputState::recordFrameInput()
{
    for ( int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END;
          ++key ) {
        const auto imguiKey = static_cast<ImGuiKey>(key);
        if ( ImGui::IsKeyPressed(imguiKey, false) ) {
            pushEvent(std::string{ "Key down: " } +
                      ImGui::GetKeyName(imguiKey));
        }
        if ( ImGui::IsKeyReleased(imguiKey) ) {
            pushEvent(std::string{ "Key up: " } + ImGui::GetKeyName(imguiKey));
        }
    }

    for ( int button = 0; button < 5; ++button ) {
        if ( ImGui::IsMouseClicked(button) ) {
            pushEvent("Mouse down: button " + std::to_string(button));
        }
        if ( ImGui::IsMouseReleased(button) ) {
            pushEvent("Mouse up: button " + std::to_string(button));
        }
    }
}

void InputState::clearEvents()
{
    m_events.clear();
}

const std::deque<std::string>& InputState::events() const
{
    return m_events;
}

void InputState::pushEvent(std::string eventText)
{
    m_events.push_front(std::move(eventText));
    while ( m_events.size() > MAX_EVENT_COUNT ) {
        m_events.pop_back();
    }
}

}  // namespace vi
