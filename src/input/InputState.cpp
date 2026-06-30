#include "input/InputState.h"
#include "input/GlobalInputListener.h"

#include <chrono>
#include <cstddef>
#include <sstream>
#include <utility>

namespace vi
{
namespace
{

/// @brief 输入事件日志最大条数。
constexpr std::size_t MAX_EVENT_COUNT = 64;

/// @brief InputListener 鼠标按钮最大编号。
constexpr int MAX_MOUSE_BUTTON = 5;

/// @brief KPS/CPS 统计窗口长度。
constexpr auto RATE_WINDOW = std::chrono::seconds(1);

/// @brief 获取键盘事件显示文本。
/// @param event 全局输入事件。
/// @return 可显示键位文本。
std::string keyLabel(const GlobalInputEvent& event)
{
    if ( !event.text.empty() ) {
        return event.text;
    }

    return "code " + std::to_string(event.code);
}

/// @brief 获取鼠标按钮显示文本。
/// @param button InputListener 鼠标按钮编号。
/// @return 可显示按钮文本。
std::string mouseButtonLabel(int button)
{
    switch ( button ) {
    case 1: return "left";
    case 2: return "right";
    case 3: return "middle";
    case 4: return "side";
    case 5: return "extra";
    default: return "button " + std::to_string(button);
    }
}

/// @brief 格式化滚动事件日志。
/// @param event 全局输入事件。
/// @return 滚动事件文本。
std::string scrollEventText(const GlobalInputEvent& event)
{
    std::ostringstream stream;
    stream << "Global mouse scroll: dx=" << event.scrollX
           << " dy=" << event.scrollY;
    return stream.str();
}

}  // namespace

void InputState::consumeGlobalInput(GlobalInputListener& globalInput)
{
    m_scrollX = 0.0;
    m_scrollY = 0.0;

    GlobalInputEvent event;
    while ( globalInput.tryDequeue(event) ) {
        applyGlobalEvent(event);
    }

    pruneRateCounters(RateClock::now());
}

void InputState::clearEvents()
{
    m_events.clear();
}

const std::deque<std::string>& InputState::events() const
{
    return m_events;
}

const std::map<int, std::string>& InputState::activeKeys() const
{
    return m_activeKeys;
}

bool InputState::isKeyDown(int rawCode) const
{
    return m_activeKeys.find(rawCode) != m_activeKeys.end();
}

double InputState::keyKps(int rawCode) const
{
    const auto it = m_keyPressTimes.find(rawCode);
    if ( it == m_keyPressTimes.end() ) {
        return 0.0;
    }

    return static_cast<double>(it->second.size());
}

double InputState::totalKeyKps() const
{
    std::size_t total = 0;
    for ( const auto& keyPressTimes : m_keyPressTimes ) {
        total += keyPressTimes.second.size();
    }

    return static_cast<double>(total);
}

bool InputState::isMouseButtonDown(int button) const
{
    if ( button < 0 || button >= static_cast<int>(m_mouseButtons.size()) ) {
        return false;
    }

    return m_mouseButtons[static_cast<std::size_t>(button)];
}

double InputState::mouseButtonCps(int button) const
{
    if ( button < 0 || button >= static_cast<int>(m_mouseClickTimes.size()) ) {
        return 0.0;
    }

    return static_cast<double>(
        m_mouseClickTimes[static_cast<std::size_t>(button)].size());
}

double InputState::totalMouseCps() const
{
    std::size_t total = 0;
    for ( const auto& clickTimes : m_mouseClickTimes ) {
        total += clickTimes.size();
    }

    return static_cast<double>(total);
}

double InputState::mouseX() const
{
    return m_mouseX;
}

double InputState::mouseY() const
{
    return m_mouseY;
}

double InputState::scrollX() const
{
    return m_scrollX;
}

double InputState::scrollY() const
{
    return m_scrollY;
}

void InputState::applyGlobalEvent(const GlobalInputEvent& event)
{
    switch ( event.type ) {
    case GlobalInputEventType::KeyPress: {
        const auto label         = keyLabel(event);
        m_activeKeys[event.code] = label;
        recordKeyPress(event.code, event.timestamp);
        pushEvent("Global key down: " + label);
        break;
    }
    case GlobalInputEventType::KeyRelease: {
        const auto label = keyLabel(event);
        m_activeKeys.erase(event.code);
        pushEvent("Global key up: " + label);
        break;
    }
    case GlobalInputEventType::KeyRepeat: {
        m_activeKeys[event.code] = keyLabel(event);
        break;
    }
    case GlobalInputEventType::MousePress: {
        if ( event.code >= 0 && event.code <= MAX_MOUSE_BUTTON ) {
            m_mouseButtons[static_cast<std::size_t>(event.code)] = true;
            recordMouseClick(event.code, event.timestamp);
        }
        pushEvent("Global mouse down: " + mouseButtonLabel(event.code));
        break;
    }
    case GlobalInputEventType::MouseRelease: {
        if ( event.code >= 0 && event.code <= MAX_MOUSE_BUTTON ) {
            m_mouseButtons[static_cast<std::size_t>(event.code)] = false;
        }
        pushEvent("Global mouse up: " + mouseButtonLabel(event.code));
        break;
    }
    case GlobalInputEventType::MouseMove:
    case GlobalInputEventType::MouseDrag: {
        m_mouseX = event.x;
        m_mouseY = event.y;
        break;
    }
    case GlobalInputEventType::MouseScroll: {
        m_mouseX = event.x;
        m_mouseY = event.y;
        m_scrollX += event.scrollX;
        m_scrollY += event.scrollY;
        pushEvent(scrollEventText(event));
        break;
    }
    }
}

void InputState::recordKeyPress(int rawCode, RateTimestamp timestamp)
{
    auto& timestamps = m_keyPressTimes[rawCode];
    timestamps.push_back(timestamp);
    pruneRateWindow(timestamps, timestamp);
}

void InputState::recordMouseClick(int button, RateTimestamp timestamp)
{
    if ( button < 0 || button >= static_cast<int>(m_mouseClickTimes.size()) ) {
        return;
    }

    auto& timestamps = m_mouseClickTimes[static_cast<std::size_t>(button)];
    timestamps.push_back(timestamp);
    pruneRateWindow(timestamps, timestamp);
}

void InputState::pruneRateCounters(RateTimestamp now)
{
    for ( auto it = m_keyPressTimes.begin(); it != m_keyPressTimes.end(); ) {
        pruneRateWindow(it->second, now);
        if ( it->second.empty() ) {
            it = m_keyPressTimes.erase(it);
            continue;
        }

        ++it;
    }

    for ( auto& timestamps : m_mouseClickTimes ) {
        pruneRateWindow(timestamps, now);
    }
}

void InputState::pruneRateWindow(std::deque<RateTimestamp>& timestamps,
                                 RateTimestamp              now)
{
    while ( !timestamps.empty() && now - timestamps.front() > RATE_WINDOW ) {
        timestamps.pop_front();
    }
}

void InputState::pushEvent(std::string eventText)
{
    m_events.push_front(std::move(eventText));
    while ( m_events.size() > MAX_EVENT_COUNT ) {
        m_events.pop_back();
    }
}

}  // namespace vi
