/*
  XBeeJoystick.h - Library for XBeeJoystick arduino code.
  Coded by Oidas Andrade.
*/
#ifndef Infravermelho_h
#define Infravermelho_h

#include <LedControl.h>
#include <IRremote.h>
#include "BaseApp.h"

#define RECEBE_INFRA 6
#define bt1 16724175
#define bt2 16718055
#define bt3 16743045
#define bt4 16716015
#define bt5 16726215
#define bt6 16734885
#define bt7 16728765
#define bt8 16730805
#define bt9 255

class Infravermelho: public BaseApp
{
  public:
    void loopApp();
    void setupApp(String appName, LedControl *ledMatrix);
    
};

#endif
