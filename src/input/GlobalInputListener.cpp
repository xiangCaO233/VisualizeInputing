#include "input/GlobalInputListener.h"

#include <utility>

namespace vi
{

GlobalInputListener::GlobalInputListener()
{
    m_keyConnection   = InputListener::GlobalScreen::addKeyListener(*this);
    m_mouseConnection = InputListener::GlobalScreen::addMouseListener(*this);

    InputListener::GlobalScreen::registerScreenHook();
    m_hookRegistered = true;
}

GlobalInputListener::~GlobalInputListener()
{
    if ( m_hookRegistered ) {
        InputListener::GlobalScreen::unRegisterScreenHook();
    }

    m_mouseConnection.disconnect();
    m_keyConnection.disconnect();
}

bool GlobalInputListener::tryDequeue(GlobalInputEvent& event)
{
    return m_events.try_dequeue(event);
}

void GlobalInputListener::onPress(const InputListener::KeyEvent& event)
{
    enqueueKeyEvent(GlobalInputEventType::KeyPress, event);
}

void GlobalInputListener::onRelease(const InputListener::KeyEvent& event)
{
    enqueueKeyEvent(GlobalInputEventType::KeyRelease, event);
}

void GlobalInputListener::onRepeat(const InputListener::KeyEvent& event)
{
    enqueueKeyEvent(GlobalInputEventType::KeyRepeat, event);
}

void GlobalInputListener::onPress(const InputListener::MouseEvent& event)
{
    enqueueMouseEvent(GlobalInputEventType::MousePress, event);
}

void GlobalInputListener::onRelease(const InputListener::MouseEvent& event)
{
    enqueueMouseEvent(GlobalInputEventType::MouseRelease, event);
}

void GlobalInputListener::onMove(const InputListener::MouseEvent& event)
{
    enqueueMouseEvent(GlobalInputEventType::MouseMove, event);
}

void GlobalInputListener::onDrag(const InputListener::MouseEvent& event)
{
    enqueueMouseEvent(GlobalInputEventType::MouseDrag, event);
}

void GlobalInputListener::onScroll(const InputListener::MouseEvent& event)
{
    enqueueMouseEvent(GlobalInputEventType::MouseScroll, event);
}

void GlobalInputListener::enqueueKeyEvent(GlobalInputEventType           type,
                                          const InputListener::KeyEvent& event)
{
    GlobalInputEvent queuedEvent;
    queuedEvent.type = type;
    queuedEvent.code = event.getRawCode();
    queuedEvent.text = event.getKey();
    m_events.enqueue(std::move(queuedEvent));
}

void GlobalInputListener::enqueueMouseEvent(
    GlobalInputEventType type, const InputListener::MouseEvent& event)
{
    GlobalInputEvent queuedEvent;
    queuedEvent.type    = type;
    queuedEvent.code    = event.getButton();
    queuedEvent.x       = event.X2D();
    queuedEvent.y       = event.Y2D();
    queuedEvent.scrollX = event.getScrollDeltaX();
    queuedEvent.scrollY = event.getScrollDeltaY();
    m_events.enqueue(std::move(queuedEvent));
}

}  // namespace vi
