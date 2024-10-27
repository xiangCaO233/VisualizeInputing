#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

shader::shader(const char *vsourcefile, const char *fsourcefile) {
  // 文件流
  std::fstream vsourcestream;
  std::fstream fsourcestream;

  // 打开文件
  vsourcestream.open(vsourcefile);
  fsourcestream.open(fsourcefile);

  // 字符流
  std::stringstream vertex_source_stream, fragment_source_stream;

  // 读取到字符流
  vertex_source_stream << vsourcestream.rdbuf();
  fragment_source_stream << fsourcestream.rdbuf();

  // 源代码字符串
  const char *vertex_source, *fragment_source;

  // 从流中读出源代码
  vertex_source = vertex_source_stream.str().c_str();
  fragment_source = fragment_source_stream.str().c_str();

  // 创建着色器
  // 顶点着色器
  GLuint vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  // 片段着色器
  GLuint fragment_shader;
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  // 导入源代码
  glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
  glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
  // 编译着色器
  glCompileShader(vertex_shader);
  // 检查编译错误
  check_compile_errors(vertex_shader, SHADER);
  glCompileShader(fragment_shader);
  check_compile_errors(fragment_shader, SHADER);

  // 创建着色器程序
  shader_program = glCreateProgram();
  // 设置着色器
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  // 链接着色器
  glLinkProgram(shader_program);

  // 释放着色器
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

shader::~shader() {
  // 释放着色器程序
  glDeleteProgram(shader_program);
}

void shader::check_compile_errors(GLuint val, ShaderType type) {
  int success;
  char infoLog[512];
  switch (type) {
  case SHADER: {
    glGetShaderiv(val, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(val, 512, nullptr, infoLog);
      std::cerr << "着色器编译失败:" << infoLog << std::endl;
    }
    break;
  }
  case PROGRAM: {
    glGetProgramiv(val, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(val, 512, nullptr, infoLog);
      std::cerr << "着色器程序链接失败:" << infoLog << std::endl;
    }
    break;
  }
  }
}

void shader::use() { glUseProgram(shader_program); }
