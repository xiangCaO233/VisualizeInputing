#ifndef MOUSEWIDGET_H
#define MOUSEWIDGET_H

#include "AlignType.h"
#include "Caller.h"
#include "KeyWidget.h"
#include <QLabel>
#include <QLayout>
#include <QWidget>

class MouseWidget : public QWidget {
  Q_OBJECT
  // 根布局vbox
  QVBoxLayout *rootLayout;

  // 鼠标状态
  static QString stateIfStr;
  QString mouseStateV{""};
  QLabel *mouseStateIf;
  // 计时消失
  QTimer *countingTimer;

  // 鼠标位置
  static QString locIfStr;
  QPointF mouseLocV{};
  QLabel *mouseLocIf;

  // 鼠标拖动状态
  static QString dragIfStr;
  QPointF fromLocV{};
  QPointF toLocV{};
  QLabel *mouseDragIf;

  // 颜色配置
  QColor bgColor;
  QColor forgeColor;
  QColor stateColor;
  QColor locColor;
  QColor dragLocColor1;
  QColor dragLocColor2;
  QColor fBgColor;

  // 字体大小
  int fontSize = 20;

  // 对齐方式配置
  AlignType aType = AlignType::RIGHTTOP;

public slots:
  void updateStatus(const char *status);
  void updateLocation(double xd, double yd);
  void updateDragIf(double fxd, double fyd, double txd, double tyd);
  void removeStatus();

public:
  // 传递器
  static Caller clickCaller;
  static Caller moveCaller;
  static Caller dragCaller;

  MouseWidget();
  ~MouseWidget();

  // 设置背景色
  void setBgColor(QColor &&color);
  // 设置前景色
  void setFgColor(QColor &&color);
  // 设置状态色
  void setStaColor(QColor &&color);
  // 设置位置颜色
  void setLocColor(QColor &&color);
  // 设置拖动坐标色1，2
  void setDragColor(QColor &&color1, QColor &&color2);
  // 设置前景色
  void setFBgColor(QColor &&color);
  // 设置对其方式
  void setAlign(AlignType t);
  // 更新位置
  void updatewLocation();
};

#endif // MOUSEWIDGET_H
