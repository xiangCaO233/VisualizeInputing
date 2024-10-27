#include <cstdio>
#include <iostream>
#include <unistd.h>
#ifdef __APPLE__
#include "gls/glfws/macos/mglwindow.h"
#endif //__APPLE__

#ifdef __unix
#include "gls/glfws/linux/lglwindow.h"
#endif //__unix

int main(int argc, char *argv[]) {
  glwindow window("test");
  usleep(3000000);
  window.set_visible(true);
  usleep(5000000);
  window.set_visible(false);
  getchar();
}
