#include "../headers/KeyWidget.h"

Caller KeyWidget::caller;
QSizeF KeyWidget::screenSize;

KeyWidget::KeyWidget() : QWidget(nullptr) {
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  setBgColor(QColor::fromRgb(33, 33, 33));
  setFgColor(QColor::fromRgb(4, 220, 220));
  setFBgColor(QColor::fromRgb(33, 33, 33));
  fadeTimer = new QTimer;
  countingTimer = new QTimer;
  countingTimer->setSingleShot(true);
  connect(countingTimer, &QTimer::timeout, this, &KeyWidget::startFade);
  connect(fadeTimer, &QTimer::timeout, this, &KeyWidget::fadeOut);
  layout = new QHBoxLayout;
  layout->setSpacing(2);
  layout->setContentsMargins(2, 2, 2, 2);
  setLayout(layout);
  connect(&caller, &Caller::sendStr, this, &KeyWidget::updateStrFading);
  screenSize = QGuiApplication::primaryScreen()->size();
}

KeyWidget::~KeyWidget() {}

// 绘制窗口
void KeyWidget::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  p.setPen(Qt::NoPen);
  p.setBrush(bgColor);
}

void KeyWidget::updateStrFading(const char *str) {
  // 接收字符串
  hide();
  auto l = new QLabel(str);
  // 使用样式表设置字体大小为 20 像素
  l->setStyleSheet(QString("QLabel { color: %1;font-size: %2px;}")
                       .arg(forgeColor.name())
                       .arg(fontSize));
  l->adjustSize();
  displayQueue.push(l);
  layout->addWidget(l);
  adjustSize();
  updateLocation();
  // 重置窗口的透明度为1
  setWindowOpacity(1.0);
  // 停止消失定时器
  fadeTimer->stop();
  show();
  countingTimer->start(382);
}

void KeyWidget::fadeOut() {
  // 获取当前透明度
  qreal opacity = this->windowOpacity();
  if (opacity > 0) {
    // 减少透明度
    setWindowOpacity(opacity - (1.0 / 37.0));
  } else {
    while (!displayQueue.empty()) {
      auto l = displayQueue.front();
      layout->removeWidget(l);
      displayQueue.pop();
      delete l;
    }
  }
}

void KeyWidget::startFade() {
  // 立即启动新的计时器
  fadeTimer->start(1000 / 60);
}

void KeyWidget::setBgColor(QColor &&color) { bgColor = color; }

void KeyWidget::setFgColor(QColor &&color) { forgeColor = color; }

void KeyWidget::setFBgColor(QColor &&color) { fBgColor = color; }

void KeyWidget::setAlign(AlignType t) { aType = t; }

void KeyWidget::setFontSize(int size) { fontSize = size; }

void KeyWidget::updateLocation() {
  switch (aType) {
  case AlignType::BOTTOM: {
    move((screenSize.width() - width()) / 2,
         screenSize.height() - height() - 10);
    break;
  };
  case AlignType::TOP: {
    move((screenSize.width() - width()) / 2, 10);
    break;
  };
  case AlignType::LEFT: {
    break;
  };
  case AlignType::RIGHT: {
    break;
  };
  case AlignType::LEFTBOTTOM: {
    break;
  };
  case AlignType::LEFTTOP: {
    break;
  };
  case AlignType::RIGHTBOTTOM: {
    break;
  };
  case AlignType::RIGHTTOP: {
    break;
  }
  }
}

#include "moc_KeyWidget.cpp"
