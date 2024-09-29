#include "../headers/MouseWidget.h"
#include "../headers/lsr/GlobalKeyListener.h"
#include "../headers/lsr/GlobalMouseListener.h"
#include "GlobalScreen.h"
#include <QApplication>
#include <iostream>
#include <ostream>

#include <objc/message.h>
#include <objc/runtime.h>

void hideDockIcon() {

  // NSApplicationActivationPolicyAccessory
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  auto l = new GlobalKeyListener;
  // auto l2 = new GlobalMouseListenr;
  GlobalScreen::registerScreenHook();
  GlobalScreen::addKeyListener(l);
  // GlobalScreen::addMouseListener(l2);
  // auto w1 = new MouseWidget;
  auto w2 = new KeyWidget;

  // w1->show();
  w2->move(0, 0);
  w2->show();

  return app.exec();
}
