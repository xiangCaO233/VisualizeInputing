#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

// 圆的绘制位置
float circleX = 0.0f;
float circleY = 0.0f;

// 鼠标移动事件回调函数
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
  // 将鼠标屏幕位置映射到窗口相对坐标 -1.0 到 1.0 的范围内
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  circleX = (xpos / width) * 2.0f - 1.0f;
  circleY = 1.0f - (ypos / height) * 2.0f; // 注意Y轴翻转
}

// 初始化着色器
GLuint createShaderProgram() {
  const char *vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        uniform vec2 offset;
        void main() {
            gl_Position = vec4(aPos + offset, 0.0, 1.0);
        }
    )";

  const char *fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0); // 红色
        }
    )";

  // 编译顶点着色器
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // 检查编译错误
  GLint success;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR: Vertex Shader Compilation Failed\n"
              << infoLog << std::endl;
  }

  // 编译片段着色器
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // 检查编译错误
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cerr << "ERROR: Fragment Shader Compilation Failed\n"
              << infoLog << std::endl;
  }

  // 链接着色器程序
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // 检查链接错误
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cerr << "ERROR: Shader Program Linking Failed\n"
              << infoLog << std::endl;
  }

  // 删除着色器
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

void setupCircleVAO(GLuint &VAO, GLuint &VBO) {
  const int segments = 100;
  const float radius = 0.05f;
  float vertices[segments * 2];
  for (int i = 0; i < segments; ++i) {
    float angle = 2.0f * M_PI * i / segments;
    vertices[i * 2] = cos(angle) * radius;
    vertices[i * 2 + 1] = sin(angle) * radius;
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
}

int main() {
  // 初始化 GLFW
  if (!glfwInit()) {
    return -1;
  }

  // 创建 OpenGL Core Profile 窗口
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window =
      glfwCreateWindow(800, 600, "Dynamic Circle Drawing", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  // 注册鼠标移动回调
  glfwSetCursorPosCallback(window, cursor_position_callback);

  // 设置着色器
  GLuint shaderProgram = createShaderProgram();

  // 设置圆的 VAO 和 VBO
  GLuint VAO, VBO;
  setupCircleVAO(VAO, VBO);

  // 主渲染循环
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    // 使用着色器程序
    glUseProgram(shaderProgram);

    // 设置偏移量
    GLint offsetLocation = glGetUniformLocation(shaderProgram, "offset");
    glUniform2f(offsetLocation, circleX, circleY);

    // 绑定 VAO 并绘制圆
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 100);

    // 交换缓冲区并轮询事件
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // 清理资源
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
