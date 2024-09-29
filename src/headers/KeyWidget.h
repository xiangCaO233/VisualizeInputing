#ifndef KEYWIDGET_H
#define KEYWIDGET_H
#include "AlignType.h"
#include "Caller.h"
#include <QApplication>
#include <QLabel>
#include <QLayout>
#include <QPainter>
#include <QScreen>
#include <QString>
#include <QTimer>
#include <QWidget>
#include <queue>

class KeyWidget : public QWidget {
  Q_OBJECT
  // 显示队列
  std::queue<QLabel *> displayQueue;
  // 布局
  QHBoxLayout *layout;

  // 颜色配置
  QColor bgColor;
  QColor forgeColor;
  QColor fBgColor;
  // 字体大小设置
  int fontSize = 32;

  // 位置配置
  AlignType aType = AlignType::BOTTOM;
  // 消失计时器
  QTimer *fadeTimer;
  // 计时消失
  QTimer *countingTimer;

public:
  explicit KeyWidget();
  ~KeyWidget() override;
  static QSizeF screenSize;
  // 线程中介
  static Caller caller;
  // 设置窗口背景色
  void setBgColor(QColor &&color);
  // 设置文字前景色
  void setFgColor(QColor &&color);
  // 设置文字背景色
  void setFBgColor(QColor &&color);
  // 设置对其方式
  void setAlign(AlignType t);
  // 设置文字大小
  void setFontSize(int size);
  // 更新位置
  void updateLocation();
public slots:
  void updateStrFading(const char *str);
  void fadeOut();
  void startFade();

protected:
  void paintEvent(QPaintEvent *event) override;
};
#endif // KEYWIDGET_H
