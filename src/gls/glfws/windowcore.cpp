#include "windowcore.h"
#include <iostream>
#include <stdexcept>

bool glwindow::_is_init{false};
void glwindow::framebuffersizecallback(GLFWwindow *window, int width,
                                       int height) {
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 调用实际的成员函数
    instance->resize_event(window, width, height);
  }
}

void glwindow::inputs(GLFWwindow *window, int key, int keycode, int action,
                      int mods) {
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 调用实际的成员函数
    instance->key_event(window, key, keycode, action, mods);
  };
}

void glwindow::mousebuttoncallback(GLFWwindow *window, int button, int action,
                                   int mods) {
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 调用实际的成员函数
    instance->mouse_event(window, button, action, mods);
  };
}

void glwindow::mousemovecallback(GLFWwindow *window, double x, double y) {
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 调用实际的成员函数
    instance->mouse_move_event(window, x, y);
  };
}

void glwindow::mousewheelcallback(GLFWwindow *window, double dx, double dy) {
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 调用实际的成员函数
    instance->mouse_wheel_event(window, dx, dy);
  };
}

glwindow::glwindow(const char *title) {
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
  // 启用gl实验性先进功能
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("glew初始化失败");
  } else {
    _is_init = true;
  }
  std::unique_lock<std::mutex> lock(mtx);
  while (thread_flag) {
    std::cout << "等待通知进入渲染循环" << std::endl;
    // 等待通知
    cv.wait(lock, [this] { return visible; });
    std::cout << "进入渲染循环" << std::endl;
    // 使其他线程能够访问到visible变量
    lock.unlock();
    // 实际渲染循环
    while (visible) {
      // 背景颜色
      glClearColor(_background_color[0], _background_color[1],
                   _background_color[2], _background_color[3]);
      // 清除颜色缓存
      glClear(GL_COLOR_BUFFER_BIT);

      glfwSwapBuffers(window);
      // 进入下一事件循环
      glfwPollEvents();
    }
    lock.lock();
  }
}

void glwindow::set_visible(bool is_visible) {
  {
    std::lock_guard<std::mutex> lock(mtx);
    visible = is_visible;
  }
  cv.notify_all();
}

void glwindow::set_msaa(int samples) {
  // 启用MSAA
  glfwWindowHint(GLFW_SAMPLES, samples);
}
int glwindow::get_max_msaa() {
  // 查询最大支持抗锯齿MSAA倍率
  GLint maxSamples;
  glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
  return maxSamples;
}

void glwindow::set_max_framerate(int max_framerate) {
  _max_framerate = max_framerate;
}

void glwindow::enable_vsync() { glfwSwapInterval(1); }

void glwindow::unable_vsync() { glfwSwapInterval(0); }

void glwindow::initialize_shader(const char *vsourcefile,
                                 const char *fsourcefile) {
  _glshader = new shader(vsourcefile, fsourcefile);
};

void glwindow::key_event(GLFWwindow *window, int key, int keycode, int action,
                         int mods) {
  // 键盘输入事件
}
void glwindow::mouse_event(GLFWwindow *window, int button, int action,
                           int mods){
    // 鼠标按下事件
};
void glwindow::mouse_move_event(GLFWwindow *window, double x, double y){
    // 鼠标移动事件
};
void glwindow::mouse_wheel_event(GLFWwindow *window, double dx, double dy){
    // 鼠标滚轮事件
};

void glwindow::resize_event(GLFWwindow *window, int w, int h) {
  // 调整大小事件
  glViewport(0, 0, w, h);
}
