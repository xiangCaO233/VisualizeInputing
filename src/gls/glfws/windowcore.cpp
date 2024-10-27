#include "windowcore.h"

glwindowcore::glwindowcore(const char *title){};

glwindowcore::~glwindowcore(){};
void glwindowcore::set_msaa(int samples) {
  // 启用MSAA
  glfwWindowHint(GLFW_SAMPLES, samples);
}
int glwindowcore::get_max_msaa() {
  // 查询最大支持抗锯齿MSAA倍率
  GLint maxSamples;
  glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
  return maxSamples;
}

void glwindowcore::set_max_framerate(int max_framerate) {
  _max_framerate = max_framerate;
}

void glwindowcore::enable_vsync() { glfwSwapInterval(1); }

void glwindowcore::unable_vsync() { glfwSwapInterval(0); }

void glwindowcore::initialize_shader(const char *vsourcefile,
                                     const char *fsourcefile) {
  _glshader = new shader(vsourcefile, fsourcefile);
};

void glwindowcore::set_visible(bool is_visible) { visible = is_visible; }

void glwindowcore::key_event(GLFWwindow *window, int key, int keycode,
                             int action, int mods) {
  // 键盘输入事件
}
void glwindowcore::mouse_event(GLFWwindow *window, int button, int action,
                               int mods){
    // 鼠标按下事件
};
void glwindowcore::mouse_move_event(GLFWwindow *window, double x, double y){
    // 鼠标移动事件
};
void glwindowcore::mouse_wheel_event(GLFWwindow *window, double dx, double dy){
    // 鼠标滚轮事件
};

void glwindowcore::resize_event(GLFWwindow *window, int w, int h) {
  // 调整大小事件
  glViewport(0, 0, w, h);
}
