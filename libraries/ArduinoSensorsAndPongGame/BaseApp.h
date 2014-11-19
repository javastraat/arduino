/*
  XBeeJoystick.h - Library for XBeeJoystick arduino code.
  Coded by Oidas Andrade.
*/
#ifndef BaseApp_h
#define BaseApp_h

#include <LedControl.h>

#define MESSAGE_SPEED 45

class BaseApp
{
  public:
    virtual void startApp();
    virtual void stopApp();
    virtual void setupApp(String appName, LedControl *ledMatrix);
    virtual void loopApp() = 0;
    
  protected:
    int _running;
    LedControl *_ledMatrix;
    String _appName;
};

#endif
