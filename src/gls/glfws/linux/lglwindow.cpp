#include "lglwindow.h"
#include <iostream>
#include <mutex>
#include <stdexcept>

bool glwindowcore::_is_init{false};
void glwindowcore::framebuffersizecallback(GLFWwindow *window, int width,
                                           int height) {
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 调用实际的成员函数
    instance->resize_event(window, width, height);
  }
}

void glwindowcore::inputs(GLFWwindow *window, int key, int keycode, int action,
                          int mods) {
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 调用实际的成员函数
    instance->key_event(window, key, keycode, action, mods);
  };
}

void glwindowcore::mousebuttoncallback(GLFWwindow *window, int button,
                                       int action, int mods) {
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 调用实际的成员函数
    instance->mouse_event(window, button, action, mods);
  };
}

void glwindowcore::mousemovecallback(GLFWwindow *window, double x, double y) {
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 调用实际的成员函数
    instance->mouse_move_event(window, x, y);
  };
}

void glwindowcore::mousewheelcallback(GLFWwindow *window, double dx,
                                      double dy) {
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 调用实际的成员函数
    instance->mouse_wheel_event(window, dx, dy);
  };
}

glwindow::glwindow(const char *title) : glwindowcore(title) {
  // 初始化glfw窗口库
  if (!_is_init) {
    if (!glfwInit()) {
      throw std::runtime_error("glfw初始化失败");
    } else {
      // 创建 OpenGL Core Profile 窗口
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
      // 苹果系统老版本opengl前向适配
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif //__APPLE__
    }
  }
  // 创建默认size窗口
  window = glfwCreateWindow(800, 600, title, NULL, NULL);
  if (!window) {
    throw std::runtime_error("窗口创建失败");
  }
  // 启用gl实验性先进功能
  glewExperimental = GL_TRUE;

  // 设置用户指针为当前实例
  glfwSetWindowUserPointer(window, this);
  // 绑定调整大小回调函数
  glfwSetFramebufferSizeCallback(window, framebuffersizecallback);
  // 绑定按键输入回调函数
  glfwSetKeyCallback(window, inputs);
  // 绑定鼠标按键回调函数
  glfwSetMouseButtonCallback(window, mousebuttoncallback);
  // 绑定鼠标移动回调函数
  glfwSetCursorPosCallback(window, mousemovecallback);
  // 绑定鼠标滚轮回调函数
  glfwSetScrollCallback(window, mousewheelcallback);

  thread_flag = true;
  // 初始化循环线程
  _loop_thread = std::thread(&glwindow::event_loop, this);
  // 分离线程
  _loop_thread.detach();
}

glwindow::~glwindow() {
  {
    std::lock_guard<std::mutex> lock(mtx);
    // 设置线程标志为false
    thread_flag = false;
  }
  // 通知线程退出
  cv.notify_all();
  if (_loop_thread.joinable()) {
    // 等待线程结束
    _loop_thread.join();
  }
  thread_flag = false;
  glfwDestroyWindow(window);
  glfwTerminate();
}

void glwindow::event_loop() {
  // 设置当前窗口为渲染线程opengl上下文(同时初始化opengl)
  glfwMakeContextCurrent(window);
  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("glew初始化失败");
  } else {
    _is_init = true;
  }
  while (thread_flag) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]() { return visible; });
    // 实际渲染循环
    while (visible && !glfwWindowShouldClose(window)) {
      // 背景颜色
      glClearColor(_background_color[0], _background_color[1],
                   _background_color[2], _background_color[3]);
      // 清除颜色缓存
      glClear(GL_COLOR_BUFFER_BIT);

      glfwSwapBuffers(window);
      // 进入下一事件循环
      glfwPollEvents();
    }
    set_visible(false);
    thread_flag = false;
    return;
  }
}

void glwindow::set_visible(bool is_visible) {
  glwindowcore::set_visible(is_visible);
  {
    if (visible) {
      // 显示窗口
      glfwShowWindow(window);
    } else {
      std::cout << "隐藏窗口" << std::endl;
      // 隐藏窗口
      glfwHideWindow(window);
    }
  }
  cv.notify_all();
}
