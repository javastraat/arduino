/*
  XBeeJoystick.h - Library for XBeeJoystick arduino code.
  Coded by Oidas Andrade.
*/
#ifndef Luz_h
#define Luz_h

#include <LedControl.h>
#include "BaseApp.h"

class Luz: public BaseApp
{
  public:
    void loopApp();
};

#endif
