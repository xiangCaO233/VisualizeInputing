#define GL_SILENCE_DEPRECATION

#include "app/Application.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cstddef>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <utility>

namespace vi
{
namespace
{

constexpr std::size_t MAX_EVENT_COUNT = 64;

void glfwErrorCallback(int error, const char* description)
{
    std::fprintf(stderr,
                 "GLFW error %d: %s\n",
                 error,
                 description == nullptr ? "unknown" : description);
}

const char* glslVersion()
{
#if defined(__APPLE__)
    return "#version 150";
#else
    return "#version 130";
#endif
}

void setupOpenGLHints()
{
#if defined(__APPLE__)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
}

}  // namespace

Application::Application(ApplicationConfig config)
    : m_config(std::move(config)), m_enableVsync(m_config.enableVsync)
{
    initialize();
}

Application::~Application()
{
    shutdown();
}

int Application::run()
{
    while ( !glfwWindowShouldClose(m_window) ) {
        glfwPollEvents();

        if ( glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0 ) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        beginFrame();
        recordFrameInput();
        renderUi();
        renderFrame();
    }

    return 0;
}

void Application::initialize()
{
    try {
        initializeGlfw();
        initializeOpenGL();
        initializeImGui();
        m_initialized = true;
    } catch ( ... ) {
        shutdown();
        throw;
    }
}

void Application::initializeGlfw()
{
    glfwSetErrorCallback(glfwErrorCallback);

    if ( glfwInit() != GLFW_TRUE ) {
        throw std::runtime_error("GLFW 初始化失败");
    }
    m_glfwInitialized = true;

    setupOpenGLHints();

    m_window = glfwCreateWindow(m_config.width,
                                m_config.height,
                                m_config.title.c_str(),
                                nullptr,
                                nullptr);
    if ( m_window == nullptr ) {
        throw std::runtime_error("GLFW 窗口创建失败");
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(m_enableVsync ? 1 : 0);
}

void Application::initializeOpenGL()
{
    if ( glGetString(GL_VERSION) == nullptr ) {
        throw std::runtime_error("OpenGL 上下文初始化失败");
    }
}

void Application::initializeImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    ImGuiStyle& style      = ImGui::GetStyle();
    style.WindowRounding   = 4.0f;
    style.FrameRounding    = 3.0f;
    style.TabRounding      = 3.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize  = 0.0f;

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glslVersion());
    m_imguiInitialized = true;
}

void Application::shutdown()
{
    if ( m_imguiInitialized ) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        m_imguiInitialized = false;
    }

    if ( m_window != nullptr ) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    if ( m_glfwInitialized ) {
        glfwTerminate();
        m_glfwInitialized = false;
    }

    glfwSetErrorCallback(nullptr);
    m_initialized = false;
}

void Application::beginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Application::renderUi()
{
    renderDockspace();
    renderMainMenu();
    renderInputMonitor();
    renderVisualizer();
    renderSettings();

    if ( m_showEventLog ) {
        renderEventLog();
    }

    if ( m_showDemoWindow ) {
        ImGui::ShowDemoWindow(&m_showDemoWindow);
    }

    if ( m_showMetricsWindow ) {
        ImGui::ShowMetricsWindow(&m_showMetricsWindow);
    }
}

void Application::renderFrame()
{
    ImGui::Render();

    int displayWidth  = 0;
    int displayHeight = 0;
    glfwGetFramebufferSize(m_window, &displayWidth, &displayHeight);
    glViewport(0, 0, displayWidth, displayHeight);
    glClearColor(
        m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window);
}

void Application::renderMainMenu()
{
    if ( !ImGui::BeginMainMenuBar() ) {
        return;
    }

    if ( ImGui::BeginMenu("File") ) {
        if ( ImGui::MenuItem("Exit") ) {
            glfwSetWindowShouldClose(m_window, GLFW_TRUE);
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

void Application::renderDockspace()
{
    ImGui::DockSpaceOverViewport(
        0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
}

void Application::renderInputMonitor()
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

void Application::renderVisualizer()
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

void Application::renderSettings()
{
    ImGui::Begin("Settings");

    if ( ImGui::Checkbox("VSync", &m_enableVsync) ) {
        glfwSwapInterval(m_enableVsync ? 1 : 0);
    }

    ImGui::ColorEdit4("Clear color", m_clearColor.data());

    const ImGuiIO& io = ImGui::GetIO();
    ImGui::Separator();
    ImGui::Text("Backend");
    ImGui::BulletText("%s", io.BackendPlatformName);
    ImGui::BulletText("%s", io.BackendRendererName);
    const auto* openGlVersion = glGetString(GL_VERSION);
    ImGui::BulletText("OpenGL %s",
                      openGlVersion == nullptr
                          ? "unknown"
                          : reinterpret_cast<const char*>(openGlVersion));

    ImGui::End();
}

void Application::renderEventLog()
{
    ImGui::Begin("Event Log", &m_showEventLog);

    if ( ImGui::Button("Clear") ) {
        m_events.clear();
    }

    ImGui::Separator();
    for ( const std::string& eventText : m_events ) {
        ImGui::TextUnformatted(eventText.c_str());
    }

    if ( ImGui::GetScrollY() >= ImGui::GetScrollMaxY() ) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::End();
}

void Application::recordFrameInput()
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

void Application::pushEvent(std::string eventText)
{
    m_events.push_front(std::move(eventText));
    while ( m_events.size() > MAX_EVENT_COUNT ) {
        m_events.pop_back();
    }
}

void Application::drawKey(const char* label, int key, float width)
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

void Application::drawKeyRow(const char* const* labels, const int* keys,
                             int count)
{
    for ( int i = 0; i < count; ++i ) {
        if ( i > 0 ) {
            ImGui::SameLine();
        }
        drawKey(labels[i], keys[i]);
    }
}

}  // namespace vi
