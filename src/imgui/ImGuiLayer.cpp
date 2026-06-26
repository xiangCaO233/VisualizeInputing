#include "imgui/ImGuiLayer.h"
#include "platform/NativeWindow.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <stdexcept>

namespace vi
{

ImGuiLayer::ImGuiLayer(NativeWindow& window)
{
    bool glfwBackendInitialized = false;
    try {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        applyStyle();

        glfwBackendInitialized =
            ImGui_ImplGlfw_InitForOpenGL(window.handle(), true);
        if ( !glfwBackendInitialized ) {
            throw std::runtime_error("ImGui GLFW backend 初始化失败");
        }

        if ( !ImGui_ImplOpenGL3_Init(glslVersion()) ) {
            throw std::runtime_error("ImGui OpenGL3 backend 初始化失败");
        }

        m_initialized = true;
    } catch ( ... ) {
        if ( glfwBackendInitialized ) {
            ImGui_ImplGlfw_Shutdown();
        }
        if ( ImGui::GetCurrentContext() != nullptr ) {
            ImGui::DestroyContext();
        }
        throw;
    }
}

ImGuiLayer::~ImGuiLayer()
{
    if ( !m_initialized ) {
        return;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    m_initialized = false;
}

void ImGuiLayer::beginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::renderDrawData()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

const char* ImGuiLayer::glslVersion() const
{
#if defined(__APPLE__)
    return "#version 150";
#else
    return "#version 130";
#endif
}

void ImGuiLayer::applyStyle()
{
    ImGui::StyleColorsDark();

    ImGuiStyle& style      = ImGui::GetStyle();
    style.WindowRounding   = 4.0f;
    style.FrameRounding    = 3.0f;
    style.TabRounding      = 3.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize  = 0.0f;
}

}  // namespace vi
