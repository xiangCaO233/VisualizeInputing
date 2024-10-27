#ifndef LGLWINDOW_H
#define LGLWINDOW_H

#include "../windowcore.h"

class glwindow : public glwindowcore {
public:
  glwindow(const char *title);
  ~glwindow() override;

protected:
  void event_loop() override;
  void set_visible(bool is_visible) override;
};

#endif // LGLWINDOW_H
