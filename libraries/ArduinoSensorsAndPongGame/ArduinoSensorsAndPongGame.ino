#include <XBeeJoystick.h>
#include <XBee.h>
#include <LedControl.h>
#include <IRremote.h>
#include <TimerOne.h>
#include <dht11.h>
#include "Pong.h"
#include "Infravermelho.h"
#include "Luz.h"
#include "Temperatura.h"
#include "Ultrassom.h"


#define DATA_IN 12
#define CLK 11
#define CS 10
#define NUMERO_MATRIZES 1

XBeeJoystick xBeeJoystick;
LedControl ledMatrix = LedControl(DATA_IN, CLK, CS, NUMERO_MATRIZES);

Pong pong;
Infravermelho infravermelho;
Luz luz;
Temperatura temperatura;
Ultrassom ultrassom;

void setup()
{   
  Serial.begin(9600);  
  setupMatrix();
  
  pong.setupApp("Pong", &ledMatrix);
  infravermelho.setupApp("Infravermelho", &ledMatrix);
  luz.setupApp("Luz", &ledMatrix);
  temperatura.setupApp("Temperatura", &ledMatrix);
  ultrassom.setupApp("Ultrassom", &ledMatrix);
}

void loop()
{  
  int joystickButtonPressed = xBeeJoystick.readJoystick();
  
  if (joystickButtonPressed)
  {
    if (joystickButtonPressed == BUTTON_A)
    {
      stopApps();
      ultrassom.startApp();     
    }      
    if (joystickButtonPressed == BUTTON_B)
    {
      stopApps();
      temperatura.startApp();
    }
    if (joystickButtonPressed == BUTTON_C)
    {
      stopApps();
      luz.startApp();
    }
    if (joystickButtonPressed == BUTTON_D)
    {
      stopApps();
      infravermelho.startApp();
    }    
    if (joystickButtonPressed == BUTTON_UP)
    {
      stopApps();
      pong.startApp();
    }    
    if (joystickButtonPressed == BUTTON_DOWN)
    {
      stopApps();
    }    
    if (joystickButtonPressed == BUTTON_LEFT || joystickButtonPressed == BUTTON_RIGHT)
    {
      pong.move(joystickButtonPressed);
    }    
  }
  
  executeAppsLoop();

}

void executeAppsLoop()
{
  pong.loopApp();
  infravermelho.loopApp();
  luz.loopApp();
  temperatura.loopApp();
  ultrassom.loopApp();
}

void stopApps()
{
  pong.stopApp();
  infravermelho.stopApp();
  luz.stopApp();
  temperatura.stopApp();
  ultrassom.stopApp();
}

void setupMatrix()
{
  ledMatrix.shutdown(0, false);
  ledMatrix.setIntensity(0, 0);
  ledMatrix.clearDisplay(0);
  
  ledMatrix.printStringScroll(0, 0, "Trabalho Final INF611", MESSAGE_SPEED, '<');
}
