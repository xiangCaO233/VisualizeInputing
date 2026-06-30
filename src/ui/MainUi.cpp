#include "ui/MainUi.h"
#include "input/InputState.h"
#include "platform/NativeWindow.h"

#include <imgui.h>

#if defined(__linux__)
#    include <linux/input-event-codes.h>
#endif

#include <algorithm>
#include <string>

#if defined(__linux__)
#    define VI_INPUT_CODE(code) code
#else
#    define VI_INPUT_CODE(code) -1
#endif

namespace vi
{
namespace
{

/// @brief InputListener 鼠标左键编号。
constexpr int mouseButtonLeft = 1;

/// @brief InputListener 鼠标右键编号。
constexpr int mouseButtonRight = 2;

/// @brief InputListener 鼠标中键编号。
constexpr int mouseButtonMiddle = 3;

/// @brief InputListener 鼠标最大展示按钮编号。
constexpr int maxMouseButton = 5;

}  // namespace

MainUi::MainUi(bool enableVsync) : m_enableVsync(enableVsync) {}

void MainUi::render(NativeWindow& window, InputState& inputState)
{
    renderMainWindow(window, inputState);

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

void MainUi::renderMainWindow(NativeWindow& window, InputState& inputState)
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 12.0f, 12.0f });

    constexpr ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;

    const bool visible =
        ImGui::Begin("VisualizeInputing", nullptr, windowFlags);
    ImGui::PopStyleVar(3);

    if ( visible ) {
        renderMainMenu(window);
        renderListenerWorkspace(window, inputState);
    }

    ImGui::End();
}

void MainUi::renderMainMenu(NativeWindow& window)
{
    if ( !ImGui::BeginMenuBar() ) {
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
    ImGui::EndMenuBar();
}

void MainUi::renderListenerWorkspace(NativeWindow& window,
                                     InputState&   inputState)
{
    const ImGuiStyle& style         = ImGui::GetStyle();
    const ImVec2      availableSize = ImGui::GetContentRegionAvail();

    float eventLogHeight = 0.0f;
    if ( m_showEventLog ) {
        eventLogHeight = std::clamp(availableSize.y * 0.32f, 140.0f, 260.0f);
        eventLogHeight =
            std::min(eventLogHeight, std::max(0.0f, availableSize.y - 140.0f));
    }

    const float monitorHeight =
        m_showEventLog
            ? std::max(0.0f,
                       availableSize.y - eventLogHeight - style.ItemSpacing.y)
            : 0.0f;

    ImGui::BeginChild("Listener Workspace", ImVec2{ 0.0f, monitorHeight });
    renderListenerPanels(window, inputState);
    ImGui::EndChild();

    if ( !m_showEventLog ) {
        return;
    }

    ImGui::BeginChild(
        "Event Log", ImVec2{ 0.0f, 0.0f }, ImGuiChildFlags_Borders);
    renderEventLog(inputState);
    ImGui::EndChild();
}

void MainUi::renderListenerPanels(NativeWindow&     window,
                                  const InputState& inputState)
{
    const ImVec2 availableSize = ImGui::GetContentRegionAvail();
    const bool   wideLayout    = availableSize.x >= 820.0f;

    if ( wideLayout && ImGui::BeginTable("Listener Panels",
                                         2,
                                         ImGuiTableFlags_Resizable |
                                             ImGuiTableFlags_SizingStretchProp,
                                         availableSize) ) {
        ImGui::TableSetupColumn(
            "Input", ImGuiTableColumnFlags_WidthFixed, 300.0f);
        ImGui::TableSetupColumn("Visualizer",
                                ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::BeginChild(
            "Input Monitor", ImVec2{ 0.0f, 0.0f }, ImGuiChildFlags_Borders);
        renderInputMonitor(inputState);
        ImGui::EndChild();

        ImGui::TableSetColumnIndex(1);
        ImGui::BeginChild(
            "Visualizer", ImVec2{ 0.0f, 0.0f }, ImGuiChildFlags_Borders);
        renderVisualizer(inputState);
        renderSettings(window);
        ImGui::EndChild();

        ImGui::EndTable();
        return;
    }

    const float inputHeight =
        std::clamp(availableSize.y * 0.34f, 150.0f, 220.0f);
    ImGui::BeginChild(
        "Input Monitor", ImVec2{ 0.0f, inputHeight }, ImGuiChildFlags_Borders);
    renderInputMonitor(inputState);
    ImGui::EndChild();

    ImGui::BeginChild(
        "Visualizer", ImVec2{ 0.0f, 0.0f }, ImGuiChildFlags_Borders);
    renderVisualizer(inputState);
    renderSettings(window);
    ImGui::EndChild();
}

void MainUi::renderInputMonitor(const InputState& inputState)
{
    ImGui::SeparatorText("Input Monitor");

    ImGui::Text("Source: InputListener global hook");
#if defined(__linux__)
    ImGui::TextDisabled(
        "Linux backend reads /dev/input/event* without window focus.");
#endif

    ImGui::Text("Mouse");
    ImGui::Text("Relative position: %.1f, %.1f",
                inputState.mouseX(),
                inputState.mouseY());
    ImGui::Text("Wheel delta: %.2f horizontal %.2f",
                inputState.scrollY(),
                inputState.scrollX());

    for ( int button = mouseButtonLeft; button <= maxMouseButton; ++button ) {
        ImGui::SameLine(button == mouseButtonLeft ? 0.0f : -1.0f);
        const bool down = inputState.isMouseButtonDown(button);
        ImGui::TextColored(down ? ImVec4{ 0.35f, 0.95f, 0.65f, 1.0f }
                                : ImVec4{ 0.55f, 0.58f, 0.62f, 1.0f },
                           "B%d",
                           button);
    }

    ImGui::Separator();
    ImGui::Text("Active keys");
    int shownKeys = 0;
    for ( const auto& activeKey : inputState.activeKeys() ) {
        if ( shownKeys > 0 ) {
            ImGui::SameLine();
        }
        ImGui::TextColored(ImVec4{ 0.35f, 0.95f, 0.65f, 1.0f },
                           "%s",
                           activeKey.second.c_str());
        ++shownKeys;
    }

    if ( shownKeys == 0 ) {
        ImGui::TextDisabled("None");
    }
}

void MainUi::renderVisualizer(const InputState& inputState)
{
    ImGui::SeparatorText("Visualizer");

    const char* row1Labels[] = { "1", "2", "3", "4", "5",
                                 "6", "7", "8", "9", "0" };
    const int   row1Keys[]   = { VI_INPUT_CODE(KEY_1), VI_INPUT_CODE(KEY_2),
                                 VI_INPUT_CODE(KEY_3), VI_INPUT_CODE(KEY_4),
                                 VI_INPUT_CODE(KEY_5), VI_INPUT_CODE(KEY_6),
                                 VI_INPUT_CODE(KEY_7), VI_INPUT_CODE(KEY_8),
                                 VI_INPUT_CODE(KEY_9), VI_INPUT_CODE(KEY_0) };
    drawKeyRow(inputState, row1Labels, row1Keys, 10);

    const char* row2Labels[] = { "Q", "W", "E", "R", "T",
                                 "Y", "U", "I", "O", "P" };
    const int   row2Keys[]   = { VI_INPUT_CODE(KEY_Q), VI_INPUT_CODE(KEY_W),
                                 VI_INPUT_CODE(KEY_E), VI_INPUT_CODE(KEY_R),
                                 VI_INPUT_CODE(KEY_T), VI_INPUT_CODE(KEY_Y),
                                 VI_INPUT_CODE(KEY_U), VI_INPUT_CODE(KEY_I),
                                 VI_INPUT_CODE(KEY_O), VI_INPUT_CODE(KEY_P) };
    drawKeyRow(inputState, row2Labels, row2Keys, 10);

    const char* row3Labels[] = { "A", "S", "D", "F", "G", "H", "J", "K", "L" };
    const int   row3Keys[]   = { VI_INPUT_CODE(KEY_A), VI_INPUT_CODE(KEY_S),
                                 VI_INPUT_CODE(KEY_D), VI_INPUT_CODE(KEY_F),
                                 VI_INPUT_CODE(KEY_G), VI_INPUT_CODE(KEY_H),
                                 VI_INPUT_CODE(KEY_J), VI_INPUT_CODE(KEY_K),
                                 VI_INPUT_CODE(KEY_L) };
    drawKeyRow(inputState, row3Labels, row3Keys, 9);

    const char* row4Labels[] = { "Z", "X", "C", "V", "B", "N", "M" };
    const int   row4Keys[]   = { VI_INPUT_CODE(KEY_Z), VI_INPUT_CODE(KEY_X),
                                 VI_INPUT_CODE(KEY_C), VI_INPUT_CODE(KEY_V),
                                 VI_INPUT_CODE(KEY_B), VI_INPUT_CODE(KEY_N),
                                 VI_INPUT_CODE(KEY_M) };
    drawKeyRow(inputState, row4Labels, row4Keys, 7);

    ImGui::Separator();
    drawKey(inputState, "Ctrl", VI_INPUT_CODE(KEY_LEFTCTRL), 56.0f);
    ImGui::SameLine();
    drawKey(inputState, "Alt", VI_INPUT_CODE(KEY_LEFTALT), 56.0f);
    ImGui::SameLine();
    drawKey(inputState, "Space", VI_INPUT_CODE(KEY_SPACE), 180.0f);
    ImGui::SameLine();
    drawKey(inputState, "Left", VI_INPUT_CODE(KEY_LEFT), 56.0f);
    ImGui::SameLine();
    drawKey(inputState, "Down", VI_INPUT_CODE(KEY_DOWN), 56.0f);
    ImGui::SameLine();
    drawKey(inputState, "Up", VI_INPUT_CODE(KEY_UP), 56.0f);
    ImGui::SameLine();
    drawKey(inputState, "Right", VI_INPUT_CODE(KEY_RIGHT), 56.0f);

    ImGui::Separator();
    drawMouseButton(inputState, "Mouse L", mouseButtonLeft, 90.0f);
    ImGui::SameLine();
    drawMouseButton(inputState, "Mouse R", mouseButtonRight, 90.0f);
    ImGui::SameLine();
    drawMouseButton(inputState, "Mouse M", mouseButtonMiddle, 90.0f);
}

void MainUi::renderSettings(NativeWindow& window)
{
    ImGui::SeparatorText("Settings");

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
}

void MainUi::renderEventLog(InputState& inputState)
{
    ImGui::SeparatorText("Event Log");

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
}

void MainUi::drawKey(const InputState& inputState, const char* label, int key,
                     float width)
{
    drawInputButton(label, inputState.isKeyDown(key), width);
}

void MainUi::drawMouseButton(const InputState& inputState, const char* label,
                             int button, float width)
{
    drawInputButton(label, inputState.isMouseButtonDown(button), width);
}

void MainUi::drawInputButton(const char* label, bool isDown, float width)
{
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

void MainUi::drawKeyRow(const InputState& inputState, const char* const* labels,
                        const int* keys, int count)
{
    for ( int i = 0; i < count; ++i ) {
        if ( i > 0 ) {
            ImGui::SameLine();
        }
        drawKey(inputState, labels[i], keys[i]);
    }
}

}  // namespace vi

#undef VI_INPUT_CODE
