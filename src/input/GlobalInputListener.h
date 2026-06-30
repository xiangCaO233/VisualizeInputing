#pragma once

#include <InputListener/InputListener.h>
#include <concurrentqueue.h>

#include <string>

namespace vi
{

/// @brief 全局输入事件类型。
enum class GlobalInputEventType {
    KeyPress,      ///< 键盘按下。
    KeyRelease,    ///< 键盘释放。
    KeyRepeat,     ///< 键盘重复。
    MousePress,    ///< 鼠标按下。
    MouseRelease,  ///< 鼠标释放。
    MouseMove,     ///< 鼠标移动。
    MouseDrag,     ///< 鼠标拖动。
    MouseScroll    ///< 鼠标滚动。
};

/// @brief 从 InputListener 回调线程传递到主线程的全局输入事件。
struct GlobalInputEvent {
    /// @brief 事件类型。
    GlobalInputEventType type{ GlobalInputEventType::KeyPress };

    /// @brief 键盘原始键码或鼠标按钮编号。
    int code{ 0 };

    /// @brief InputListener 提供的可显示键位文本。
    std::string text;

    /// @brief 鼠标相对位置 x。
    double x{ 0.0 };

    /// @brief 鼠标相对位置 y。
    double y{ 0.0 };

    /// @brief 本次滚动事件的水平增量。
    double scrollX{ 0.0 };

    /// @brief 本次滚动事件的垂直增量。
    double scrollY{ 0.0 };
};

/// @brief InputListener 全局监听器，把跨线程回调转为主线程可消费队列。
class GlobalInputListener final : public InputListener::KeyListener,
                                  public InputListener::MouseListener
{
public:
    /// @brief 注册键盘和鼠标监听器并启动全局 hook。
    GlobalInputListener();

    /// @brief 停止全局 hook 并解绑监听器。
    ~GlobalInputListener() override;

    GlobalInputListener(const GlobalInputListener&)            = delete;
    GlobalInputListener& operator=(const GlobalInputListener&) = delete;

    /// @brief 尝试取出一个全局输入事件。
    /// @param event 接收事件的输出参数。
    /// @return 队列中存在事件时返回 true。
    [[nodiscard]] bool tryDequeue(GlobalInputEvent& event);

private:
    /// @brief 处理全局键盘按下回调。
    /// @param event InputListener 键盘事件。
    void onPress(const InputListener::KeyEvent& event) override;

    /// @brief 处理全局键盘释放回调。
    /// @param event InputListener 键盘事件。
    void onRelease(const InputListener::KeyEvent& event) override;

    /// @brief 处理全局键盘重复回调。
    /// @param event InputListener 键盘事件。
    void onRepeat(const InputListener::KeyEvent& event) override;

    /// @brief 处理全局鼠标按下回调。
    /// @param event InputListener 鼠标事件。
    void onPress(const InputListener::MouseEvent& event) override;

    /// @brief 处理全局鼠标释放回调。
    /// @param event InputListener 鼠标事件。
    void onRelease(const InputListener::MouseEvent& event) override;

    /// @brief 处理全局鼠标移动回调。
    /// @param event InputListener 鼠标事件。
    void onMove(const InputListener::MouseEvent& event) override;

    /// @brief 处理全局鼠标拖动回调。
    /// @param event InputListener 鼠标事件。
    void onDrag(const InputListener::MouseEvent& event) override;

    /// @brief 处理全局鼠标滚动回调。
    /// @param event InputListener 鼠标事件。
    void onScroll(const InputListener::MouseEvent& event) override;

    /// @brief 把键盘事件压入跨线程队列。
    /// @param type 全局输入事件类型。
    /// @param event InputListener 键盘事件。
    void enqueueKeyEvent(GlobalInputEventType           type,
                         const InputListener::KeyEvent& event);

    /// @brief 把鼠标事件压入跨线程队列。
    /// @param type 全局输入事件类型。
    /// @param event InputListener 鼠标事件。
    void enqueueMouseEvent(GlobalInputEventType             type,
                           const InputListener::MouseEvent& event);

    /// @brief 回调线程到主线程的无锁事件队列。
    moodycamel::ConcurrentQueue<GlobalInputEvent> m_events;

    /// @brief 键盘监听器连接句柄。
    InputListener::ListenerConnection m_keyConnection;

    /// @brief 鼠标监听器连接句柄。
    InputListener::ListenerConnection m_mouseConnection;

    /// @brief 是否已请求启动全局 hook。
    bool m_hookRegistered{ false };
};

}  // namespace vi
