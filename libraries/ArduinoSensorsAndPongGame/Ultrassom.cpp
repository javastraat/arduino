/*
  XBeeJoystick.h - Library for XBeeJoystick arduino code.
  Coded by Oidas Andrade.
*/
#include "Ultrassom.h"

#define ECHO 3
#define TRIGGER 4

char distancia[4];

void Ultrassom::loopApp()
{  
  if (_running)
  {
    digitalWrite(TRIGGER, LOW);  
    delayMicroseconds(2);
    
    _ledMatrix->printStringScroll(0, 0, "Distancia (Centimetros): ", MESSAGE_SPEED, '<');
  
    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER, LOW);
  
    calcularDidstancia(pulseIn(ECHO,HIGH));
  
    _ledMatrix->printStringScroll(0, 0, distancia, MESSAGE_SPEED, '<');

    delay(1000);
  }
}

void Ultrassom::setupApp(String appName, LedControl *ledMatrix)
{
  BaseApp::setupApp(appName, ledMatrix);
  
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
}

void calcularDidstancia(long time)
{
  long distanceCalc = ((time /29) / 2);
  dtostrf(distanceCalc, 2, 0, distancia);
}
