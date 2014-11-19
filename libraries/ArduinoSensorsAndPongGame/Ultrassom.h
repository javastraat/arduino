/*
  XBeeJoystick.h - Library for XBeeJoystick arduino code.
  Coded by Oidas Andrade.
*/
#ifndef Ultrassom_h
#define Ultrassom_h

#include <LedControl.h>
#include "BaseApp.h"

class Ultrassom: public BaseApp
{
  public:
    void loopApp();
    void setupApp(String appName, LedControl *ledMatrix);
};

void calcularDidstancia(long time);
#endif
