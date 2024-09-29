#ifndef GLOBALMOUSELISTENER_H
#define GLOBALMOUSELISTENER_H

#include "listener/MouseListener.h"

class GlobalMouseListenr : public MouseListener {
  static double pressX;
  static double pressY;
  void onPress(const MouseEvent &e);
  void onRelease(const MouseEvent &e);
  void onMove(const MouseEvent &e);
  void onDrag(const MouseEvent &e);
  void onPressed(const MouseEvent &e);
  void onDragged(const MouseEvent &e);
};

#endif // GLOBALMOUSELISTENER_H
