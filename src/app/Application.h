#pragma once

#include <array>
#include <deque>
#include <string>

struct GLFWwindow;

namespace vi
{

struct ApplicationConfig {
    std::string title{ "VisualizeInputing" };
    int         width{ 1280 };
    int         height{ 800 };
    bool        enableVsync{ true };
};

class Application
{
public:
    explicit Application(ApplicationConfig config = {});
    ~Application();

    Application(const Application&)            = delete;
    Application& operator=(const Application&) = delete;

    int run();

private:
    void initialize();
    void initializeGlfw();
    void initializeOpenGL();
    void initializeImGui();
    void shutdown();

    void beginFrame();
    void renderUi();
    void renderFrame();

    void renderMainMenu();
    void renderDockspace();
    void renderInputMonitor();
    void renderVisualizer();
    void renderSettings();
    void renderEventLog();

    void recordFrameInput();
    void pushEvent(std::string eventText);
    void drawKey(const char* label, int key, float width = 44.0f);
    void drawKeyRow(const char* const* labels, const int* keys, int count);

    ApplicationConfig       m_config;
    GLFWwindow*             m_window{ nullptr };
    bool                    m_initialized{ false };
    bool                    m_glfwInitialized{ false };
    bool                    m_imguiInitialized{ false };
    bool                    m_showDemoWindow{ false };
    bool                    m_showMetricsWindow{ false };
    bool                    m_enableVsync{ true };
    bool                    m_showEventLog{ true };
    std::array<float, 4>    m_clearColor{ 0.08f, 0.09f, 0.10f, 1.0f };
    std::deque<std::string> m_events;
};

}  // namespace vi
