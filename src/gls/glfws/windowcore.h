#ifndef WINDOWCORE_H
#define WINDOWCORE_H

#include "../shader.h"
#include <condition_variable>
#include <mutex>
#include <thread>

class glwindow {
  // 着色器
  shader *_glshader;
  // 是否已初始化
  static bool _is_init;
  // glfw窗口指针
  GLFWwindow *window;
  // 是否显示
  bool visible{false};
  // 线程标识
  bool thread_flag{false};
  // 窗口事件循环线程
  std::thread _loop_thread;
  // 线程锁
  std::mutex mtx;
  // 条件变量
  std::condition_variable cv;
  // 最大帧率
  int _max_framerate{60};

  // 背景颜色
  float _background_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};

  // 事件循环
  void event_loop();

  // 静态回调函数
  static void framebuffersizecallback(GLFWwindow *window, int width,
                                      int height);
  static void inputs(GLFWwindow *window, int key, int keycode, int action,
                     int mods);
  static void mousebuttoncallback(GLFWwindow *window, int button, int action,
                                  int mods);
  static void mousemovecallback(GLFWwindow *window, double x, double y);
  static void mousewheelcallback(GLFWwindow *window, double dx, double dy);

public:
  glwindow(const char *title);
  ~glwindow();

  void set_visible(bool is_visible);
  // 设置msaa抗锯齿倍率
  void set_msaa(int samples);
  // 获取支持的最大msaa倍率
  int get_max_msaa();

  // 直接设置最大帧率
  void set_max_framerate(int framerate);

  // 设置垂直同步(将会无视最大帧率设置)
  void enable_vsync();
  void unable_vsync();

  // 初始化着色器(默认路径./assets/shader/...~.glsl)
  void initialize_shader(
      const char *vshadersource = "./assets/shader/vertex.glsl",
      const char *fshadersource = "./assets/shader/fragment.glsl");

  // 直接访问着色器
  inline shader *current_shader() { return _glshader; }

protected:
  void key_event(GLFWwindow *window, int key, int keycode, int action,
                 int mods);
  void mouse_event(GLFWwindow *window, int button, int action, int mods);
  void mouse_move_event(GLFWwindow *window, double x, double y);
  void mouse_wheel_event(GLFWwindow *window, double dx, double dy);
  void resize_event(GLFWwindow *window, int w, int h);
};

#endif // WINDOWCORE_H
