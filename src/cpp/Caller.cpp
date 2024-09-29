#include "../headers/Caller.h"

void Caller::transmitStr(const char *str) { emit sendStr(str); }

void Caller::transmitLoc(double x, double y) { emit sendLoc(x, y); }

void Caller::transmit2Loc(double x1, double y1, double x2, double y2) {
  emit send2Loc(x1, y1, x2, y2);
}
#include "moc_Caller.cpp"
