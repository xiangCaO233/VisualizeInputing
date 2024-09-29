#ifndef GLOBALKEYLISTENER_H
#define GLOBALKEYLISTENER_H

#include "listener/KeyListener.h"

class GlobalKeyListener : public KeyListener {
public:
  void onPress(const KeyEvent &e);
  void onRelease(const KeyEvent &e);
  void onPressed(const KeyEvent &e);
};
#endif // GLOBALKEYLISTENER_H
