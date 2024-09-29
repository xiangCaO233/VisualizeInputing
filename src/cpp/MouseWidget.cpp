#include "../headers/MouseWidget.h"
#include "Caller.h"

Caller MouseWidget::clickCaller;
Caller MouseWidget::moveCaller;
Caller MouseWidget::dragCaller;

QString MouseWidget::stateIfStr;
QString MouseWidget::locIfStr;
QString MouseWidget::dragIfStr;

MouseWidget::MouseWidget() : QWidget(nullptr) {
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  // 初始化布局
  rootLayout = new QVBoxLayout;
  rootLayout->setSpacing(2);
  rootLayout->setContentsMargins(2, 2, 2, 2);
  setLayout(rootLayout);

  // 初始化颜色
  setBgColor(Qt::transparent);
  setFgColor(QColor::fromRgb(224, 255, 255)); // 浅青色
  setFBgColor(Qt::transparent);
  setStaColor(Qt::green);
  setLocColor(QColor::fromRgb(0, 191, 255)); // 深天蓝
  setDragColor(QColor::fromRgb(255, 215, 0),
               QColor::fromRgb(153, 50, 204)); // 金色-浅紫

  QString pucPre = "<font color='" + forgeColor.name() + "'>";
  QString pucTail = "</font>";

  static QString tempStr("");
  tempStr.append(pucPre);
  tempStr.append("当前鼠标状态:");
  tempStr.append(pucTail);

  stateIfStr = tempStr.toUtf8().constData();
  mouseStateIf = new QLabel(stateIfStr);

  static QString tempStr2("");
  tempStr2.append(pucPre);
  tempStr2.append("当前鼠标位置:");
  tempStr2.append(pucTail);
  locIfStr = tempStr2.toUtf8().constData();
  mouseLocIf = new QLabel(locIfStr);

  static QString tempStr3("");
  tempStr3.append(pucPre);
  tempStr3.append("拖动从:");
  tempStr3.append(pucTail);

  dragIfStr = tempStr3.toUtf8().constData();
  mouseDragIf = new QLabel(dragIfStr);

  rootLayout->addWidget(mouseLocIf);
  rootLayout->addWidget(mouseDragIf);
  rootLayout->addWidget(mouseStateIf);

  countingTimer = new QTimer;

  connect(&clickCaller, &Caller::sendStr, this, &MouseWidget::updateStatus);
  connect(&moveCaller, &Caller::sendLoc, this, &MouseWidget::updateLocation);
  connect(&dragCaller, &Caller::send2Loc, this, &MouseWidget::updateDragIf);
  connect(countingTimer, &QTimer::timeout, this, &MouseWidget::removeStatus);

  updatewLocation();
}

MouseWidget::~MouseWidget(){};

void MouseWidget::updateStatus(const char *status) {
  mouseStateV = status;
  QString tempStr(stateIfStr);
  tempStr.append("<font color='" + stateColor.name() + "'>");
  tempStr.append(status);
  tempStr.append("</font>");
  hide();
  rootLayout->addWidget(mouseStateIf);
  mouseStateIf->setText(tempStr.toUtf8().constData());
  mouseStateIf->adjustSize();
  adjustSize();
  updatewLocation();
  show();
  countingTimer->stop();
  mouseStateIf->show();
  countingTimer->start(666);
}

void MouseWidget::updateLocation(double xd, double yd) {
  mouseLocV.setX(xd);
  mouseLocV.setY(yd);

  QString tempStr(locIfStr);
  tempStr.append("<font color='" + locColor.name() + "'>");
  tempStr.append("[" + QString::number(mouseLocV.x()) + "," +
                 QString::number(mouseLocV.y()) + "]");
  tempStr.append("</font>");
  hide();
  mouseLocIf->setText(tempStr.toUtf8().constData());
  mouseLocIf->adjustSize();
  adjustSize();
  updatewLocation();
  show();
}

void MouseWidget::updateDragIf(double fxd, double fyd, double txd, double tyd) {
  fromLocV.setX(fxd);
  fromLocV.setY(fyd);
  toLocV.setX(txd);
  toLocV.setY(tyd);

  QString tempStr(dragIfStr);
  tempStr.append("<font color='" + dragLocColor1.name() + "'>");
  tempStr.append("[" + QString::number(fromLocV.x()) + "," +
                 QString::number(fromLocV.y()) + "]</font>");
  tempStr.append("<font color='" + forgeColor.name() + "'>--到:</font>");
  tempStr.append("<font color='" + dragLocColor1.name() + "'>");

  tempStr.append("[" + QString::number(toLocV.x()) + "," +
                 QString::number(toLocV.y()) + "]</font>");

  hide();
  mouseDragIf->setText(tempStr.toUtf8().constData());
  mouseDragIf->adjustSize();
  adjustSize();
  updatewLocation();
  show();
}

void MouseWidget::removeStatus() {
  rootLayout->removeWidget(mouseStateIf);
  mouseStateIf->hide();
}

void MouseWidget::setBgColor(QColor &&color) { bgColor = color; }

void MouseWidget::setFgColor(QColor &&color) { forgeColor = color; }

void MouseWidget::setFBgColor(QColor &&color) { fBgColor = color; }

void MouseWidget::setStaColor(QColor &&color) { stateColor = color; }

void MouseWidget::setLocColor(QColor &&color) { locColor = color; }

void MouseWidget::setDragColor(QColor &&color1, QColor &&color2) {
  dragLocColor1 = color1;
  dragLocColor2 = color2;
}

void MouseWidget::updatewLocation() {
  move((KeyWidget::screenSize.width() - width() - 10), 10);
}

#include "moc_MouseWidget.cpp"
