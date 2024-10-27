#ifndef SHADER_H
#define SHADER_H

#include "glcore.h"

enum ShaderType { PROGRAM, SHADER };
class shader {
  // 着色器程序
  GLuint shader_program;

  // 检查编译或链接错误
  void check_compile_errors(GLuint val, ShaderType type);

public:
  shader(const char *vsourcefile, const char *fsourcefile);
  ~shader();

  // 使用当前着色器程序
  void use();
};

#endif // SHADER_H
