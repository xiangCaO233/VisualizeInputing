#ifndef FADINGCALLER_H
#define FADINGCALLER_H

#include <QObject>

class Caller : public QObject {
  Q_OBJECT
public:
  void transmitStr(const char *str);
  void transmitLoc(double x, double y);
  void transmit2Loc(double x1, double y1, double x2, double y2);
signals:
  void sendStr(const char *str);
  void sendLoc(double x, double y);
  void send2Loc(double x1, double y1, double x2, double y2);
};

#endif // FADINGCALLER_H
