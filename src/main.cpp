#include "gls/glfws/windowcore.h"
#include <cstdio>
#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[]) {
  glwindow window("test");
  usleep(3000000);
  window.set_visible(true);
  getchar();
}
