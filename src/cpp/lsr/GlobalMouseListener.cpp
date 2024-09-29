#include "../headers/lsr/GlobalMouseListener.h"

#include "../headers/MouseWidget.h"

double GlobalMouseListenr::pressX;
double GlobalMouseListenr::pressY;

void GlobalMouseListenr::onPress(const MouseEvent &e) {
  int button = e.getButton();
  QString tempStr("");

  pressX = e.X2D();
  pressY = e.Y2D();
  tempStr.append(
      (e.getButton() == 1 ? "左键" : (e.getButton() == 2 ? "右键" : "其他")));
  tempStr.append(" 按下");

  MouseWidget::clickCaller.transmitStr(tempStr.toUtf8().constData());
}

void GlobalMouseListenr::onRelease(const MouseEvent &e) {
  int button = e.getButton();
  QString tempStr("");

  tempStr.append(
      (e.getButton() == 1 ? "左键" : (e.getButton() == 2 ? "右键" : "其他")));
  tempStr.append(" 释放");
  MouseWidget::clickCaller.transmitStr(tempStr.toUtf8().constData());
}

void GlobalMouseListenr::onMove(const MouseEvent &e) {
  MouseWidget::moveCaller.transmitLoc(e.X2D(), e.Y2D());
}

void GlobalMouseListenr::onDrag(const MouseEvent &e) {
  MouseWidget::dragCaller.transmit2Loc(pressX, pressY, e.X2D(), e.X2D());
}

void GlobalMouseListenr::onPressed(const MouseEvent &e) {}

void GlobalMouseListenr::onDragged(const MouseEvent &e) {}
