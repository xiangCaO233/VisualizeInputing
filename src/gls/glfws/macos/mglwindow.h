#ifndef MGLWINDOW_H
#define MGLWINDOW_H

#include "../windowcore.h"

class glwindow : public glwindowcore {
public:
  glwindow(const char *title);
  ~glwindow();

private:
  void event_loop() override;
};

#endif // MGLWINDOW_H
