#include "ui/MainUi.h"
#include "input/InputState.h"
#include "platform/NativeWindow.h"

#include <imgui.h>

#include <string>

namespace vi
{

MainUi::MainUi(bool enableVsync) : m_enableVsync(enableVsync) {}

void MainUi::render(NativeWindow& window, InputState& inputState)
{
    renderDockspace();
    renderMainMenu(window);
    renderInputMonitor();
    renderVisualizer();
    renderSettings(window);

    if ( m_showEventLog ) {
        renderEventLog(inputState);
    }

    if ( m_showDemoWindow ) {
        ImGui::ShowDemoWindow(&m_showDemoWindow);
    }

    if ( m_showMetricsWindow ) {
        ImGui::ShowMetricsWindow(&m_showMetricsWindow);
    }
}

const std::array<float, 4>& MainUi::clearColor() const
{
    return m_clearColor;
}

void MainUi::renderMainMenu(NativeWindow& window)
{
    if ( !ImGui::BeginMainMenuBar() ) {
        return;
    }

    if ( ImGui::BeginMenu("File") ) {
        if ( ImGui::MenuItem("Exit") ) {
            window.requestClose();
        }
        ImGui::EndMenu();
    }

    if ( ImGui::BeginMenu("View") ) {
        ImGui::MenuItem("Event Log", nullptr, &m_showEventLog);
        ImGui::MenuItem("ImGui Demo", nullptr, &m_showDemoWindow);
        ImGui::MenuItem("ImGui Metrics", nullptr, &m_showMetricsWindow);
        ImGui::EndMenu();
    }

    ImGui::Separator();
    ImGui::Text("FPS %.1f", ImGui::GetIO().Framerate);
    ImGui::EndMainMenuBar();
}

void MainUi::renderDockspace()
{
    ImGui::DockSpaceOverViewport(
        0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
}

void MainUi::renderInputMonitor()
{
    ImGui::Begin("Input Monitor");

    const ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Mouse");
    ImGui::Text("Position: %.1f, %.1f", io.MousePos.x, io.MousePos.y);
    ImGui::Text("Wheel: %.2f horizontal %.2f", io.MouseWheel, io.MouseWheelH);

    for ( int button = 0; button < 5; ++button ) {
        ImGui::SameLine(button == 0 ? 0.0f : -1.0f);
        const bool down = ImGui::IsMouseDown(button);
        ImGui::TextColored(down ? ImVec4{ 0.35f, 0.95f, 0.65f, 1.0f }
                                : ImVec4{ 0.55f, 0.58f, 0.62f, 1.0f },
                           "B%d",
                           button);
    }

    ImGui::Separator();
    ImGui::Text("Active keys");
    int shownKeys = 0;
    for ( int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END;
          ++key ) {
        const auto imguiKey = static_cast<ImGuiKey>(key);
        if ( !ImGui::IsKeyDown(imguiKey) ) {
            continue;
        }

        if ( shownKeys > 0 ) {
            ImGui::SameLine();
        }
        ImGui::TextColored(ImVec4{ 0.35f, 0.95f, 0.65f, 1.0f },
                           "%s",
                           ImGui::GetKeyName(imguiKey));
        ++shownKeys;
    }

    if ( shownKeys == 0 ) {
        ImGui::TextDisabled("None");
    }

    ImGui::End();
}

void MainUi::renderVisualizer()
{
    ImGui::Begin("Visualizer");

    const char* row1Labels[] = { "1", "2", "3", "4", "5",
                                 "6", "7", "8", "9", "0" };
    const int   row1Keys[]   = { ImGuiKey_1, ImGuiKey_2, ImGuiKey_3, ImGuiKey_4,
                                 ImGuiKey_5, ImGuiKey_6, ImGuiKey_7, ImGuiKey_8,
                                 ImGuiKey_9, ImGuiKey_0 };
    drawKeyRow(row1Labels, row1Keys, 10);

    const char* row2Labels[] = { "Q", "W", "E", "R", "T",
                                 "Y", "U", "I", "O", "P" };
    const int   row2Keys[]   = { ImGuiKey_Q, ImGuiKey_W, ImGuiKey_E, ImGuiKey_R,
                                 ImGuiKey_T, ImGuiKey_Y, ImGuiKey_U, ImGuiKey_I,
                                 ImGuiKey_O, ImGuiKey_P };
    drawKeyRow(row2Labels, row2Keys, 10);

    const char* row3Labels[] = { "A", "S", "D", "F", "G", "H", "J", "K", "L" };
    const int   row3Keys[]   = { ImGuiKey_A, ImGuiKey_S, ImGuiKey_D,
                                 ImGuiKey_F, ImGuiKey_G, ImGuiKey_H,
                                 ImGuiKey_J, ImGuiKey_K, ImGuiKey_L };
    drawKeyRow(row3Labels, row3Keys, 9);

    const char* row4Labels[] = { "Z", "X", "C", "V", "B", "N", "M" };
    const int   row4Keys[]   = { ImGuiKey_Z, ImGuiKey_X, ImGuiKey_C, ImGuiKey_V,
                                 ImGuiKey_B, ImGuiKey_N, ImGuiKey_M };
    drawKeyRow(row4Labels, row4Keys, 7);

    ImGui::Separator();
    drawKey("Ctrl", ImGuiKey_LeftCtrl, 56.0f);
    ImGui::SameLine();
    drawKey("Alt", ImGuiKey_LeftAlt, 56.0f);
    ImGui::SameLine();
    drawKey("Space", ImGuiKey_Space, 180.0f);
    ImGui::SameLine();
    drawKey("Left", ImGuiKey_LeftArrow, 56.0f);
    ImGui::SameLine();
    drawKey("Down", ImGuiKey_DownArrow, 56.0f);
    ImGui::SameLine();
    drawKey("Up", ImGuiKey_UpArrow, 56.0f);
    ImGui::SameLine();
    drawKey("Right", ImGuiKey_RightArrow, 56.0f);

    ImGui::Separator();
    drawKey("Mouse L", ImGuiKey_MouseLeft, 90.0f);
    ImGui::SameLine();
    drawKey("Mouse R", ImGuiKey_MouseRight, 90.0f);
    ImGui::SameLine();
    drawKey("Mouse M", ImGuiKey_MouseMiddle, 90.0f);

    ImGui::End();
}

void MainUi::renderSettings(NativeWindow& window)
{
    ImGui::Begin("Settings");

    if ( ImGui::Checkbox("VSync", &m_enableVsync) ) {
        window.setVsync(m_enableVsync);
    }

    ImGui::ColorEdit4("Clear color", m_clearColor.data());

    const ImGuiIO& io = ImGui::GetIO();
    ImGui::Separator();
    ImGui::Text("Backend");
    ImGui::BulletText("%s", io.BackendPlatformName);
    ImGui::BulletText("%s", io.BackendRendererName);
    ImGui::BulletText("OpenGL %s", window.openGlVersion());

    ImGui::End();
}

void MainUi::renderEventLog(InputState& inputState)
{
    ImGui::Begin("Event Log", &m_showEventLog);

    if ( ImGui::Button("Clear") ) {
        inputState.clearEvents();
    }

    ImGui::Separator();
    for ( const std::string& eventText : inputState.events() ) {
        ImGui::TextUnformatted(eventText.c_str());
    }

    if ( ImGui::GetScrollY() >= ImGui::GetScrollMaxY() ) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::End();
}

void MainUi::drawKey(const char* label, int key, float width)
{
    const bool isDown = ImGui::IsKeyDown(static_cast<ImGuiKey>(key));
    if ( isDown ) {
        ImGui::PushStyleColor(ImGuiCol_Button,
                              ImVec4{ 0.20f, 0.56f, 0.36f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4{ 0.24f, 0.64f, 0.42f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImVec4{ 0.30f, 0.74f, 0.50f, 1.0f });
    }

    ImGui::Button(label, ImVec2{ width, 36.0f });

    if ( isDown ) {
        ImGui::PopStyleColor(3);
    }
}

void MainUi::drawKeyRow(const char* const* labels, const int* keys, int count)
{
    for ( int i = 0; i < count; ++i ) {
        if ( i > 0 ) {
            ImGui::SameLine();
        }
        drawKey(labels[i], keys[i]);
    }
}

}  // namespace vi
