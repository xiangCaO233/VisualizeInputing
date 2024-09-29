#include "../../headers/lsr/GlobalKeyListener.h"
#include "../../headers/KeyWidget.h"

void GlobalKeyListener::onPress(const KeyEvent &e) {
  //  std::cout << "当前按下:" << e.getKey() << std::endl;
  KeyWidget::caller.transmitStr(e.getKey());
}

void GlobalKeyListener::onRelease(const KeyEvent &e) {}

void GlobalKeyListener::onPressed(const KeyEvent &e) {}
