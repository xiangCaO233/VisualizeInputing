#define GL_SILENCE_DEPRECATION

#include "app/AppLoop.h"
#include "imgui/ImGuiLayer.h"
#include "input/InputState.h"
#include "platform/NativeWindow.h"
#include "ui/MainUi.h"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <chrono>
#include <thread>

namespace vi
{

AppLoop::AppLoop(NativeWindow& window, ImGuiLayer& imguiLayer, MainUi& mainUi,
                 InputState& inputState)
    : m_window(window)
    , m_imguiLayer(imguiLayer)
    , m_mainUi(mainUi)
    , m_inputState(inputState)
{
}

int AppLoop::run()
{
    while ( !m_window.shouldClose() ) {
        m_window.pollEvents();

        if ( m_window.isIconified() ) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        m_imguiLayer.beginFrame();
        m_inputState.recordFrameInput();
        m_mainUi.render(m_window, m_inputState);
        renderFrame();
    }

    return 0;
}

void AppLoop::renderFrame()
{
    ImGui::Render();

    int displayWidth  = 0;
    int displayHeight = 0;
    m_window.getFramebufferSize(displayWidth, displayHeight);
    glViewport(0, 0, displayWidth, displayHeight);

    const auto& clearColor = m_mainUi.clearColor();
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    m_imguiLayer.renderDrawData();
    m_window.swapBuffers();
}

}  // namespace vi
