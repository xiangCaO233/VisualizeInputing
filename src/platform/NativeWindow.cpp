#define GL_SILENCE_DEPRECATION

#include "platform/NativeWindow.h"

#include <GLFW/glfw3.h>

#include <cstdio>
#include <stdexcept>
#include <utility>

namespace vi
{
namespace
{

/// @brief GLFW 全局错误回调，只记录错误，不跨 C 回调边界抛异常。
/// @param error GLFW 错误码。
/// @param description GLFW 错误描述。
void glfwErrorCallback(int error, const char* description)
{
    std::fprintf(stderr,
                 "GLFW error %d: %s\n",
                 error,
                 description == nullptr ? "unknown" : description);
}

}  // namespace

NativeWindow::NativeWindow(NativeWindowConfig config)
    : m_config(std::move(config))
{
    try {
        initialize();
    } catch ( ... ) {
        shutdown();
        throw;
    }
}

NativeWindow::~NativeWindow()
{
    shutdown();
}

bool NativeWindow::shouldClose() const
{
    return glfwWindowShouldClose(m_window) == GLFW_TRUE;
}

void NativeWindow::requestClose()
{
    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

void NativeWindow::pollEvents() const
{
    glfwPollEvents();
}

bool NativeWindow::isIconified() const
{
    return glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0;
}

void NativeWindow::swapBuffers() const
{
    glfwSwapBuffers(m_window);
}

void NativeWindow::setVsync(bool enabled) const
{
    glfwSwapInterval(enabled ? 1 : 0);
}

void NativeWindow::getFramebufferSize(int& width, int& height) const
{
    glfwGetFramebufferSize(m_window, &width, &height);
}

GLFWwindow* NativeWindow::handle() const
{
    return m_window;
}

const char* NativeWindow::openGlVersion() const
{
    const auto* version = glGetString(GL_VERSION);
    return version == nullptr ? "unknown"
                              : reinterpret_cast<const char*>(version);
}

void NativeWindow::initialize()
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
    setVsync(m_config.enableVsync);
    validateOpenGLContext();
}

void NativeWindow::shutdown()
{
    if ( m_window != nullptr ) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    if ( m_glfwInitialized ) {
        glfwTerminate();
        m_glfwInitialized = false;
    }

    glfwSetErrorCallback(nullptr);
}

void NativeWindow::setupOpenGLHints() const
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

void NativeWindow::validateOpenGLContext() const
{
    if ( glGetString(GL_VERSION) == nullptr ) {
        throw std::runtime_error("OpenGL 上下文初始化失败");
    }
}

}  // namespace vi
