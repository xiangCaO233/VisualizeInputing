#include "app/Application.h"
#include "app/AppLoop.h"
#include "imgui/ImGuiLayer.h"
#include "input/GlobalInputListener.h"
#include "input/InputState.h"
#include "platform/NativeWindow.h"
#include "ui/MainUi.h"

#include <utility>

namespace vi
{

Application::Application(ApplicationConfig config) : m_config(std::move(config))
{
}

int Application::run()
{
    NativeWindowConfig windowConfig;
    windowConfig.title       = m_config.title;
    windowConfig.width       = m_config.width;
    windowConfig.height      = m_config.height;
    windowConfig.enableVsync = m_config.enableVsync;

    NativeWindow        window(windowConfig);
    ImGuiLayer          imguiLayer(window);
    InputState          inputState;
    GlobalInputListener globalInputListener;
    MainUi              mainUi(m_config.enableVsync);
    AppLoop loop(window, imguiLayer, mainUi, inputState, globalInputListener);

    return loop.run();
}

}  // namespace vi
