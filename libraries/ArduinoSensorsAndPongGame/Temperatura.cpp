/*
  XBeeJoystick.h - Library for XBeeJoystick arduino code.
  Coded by Oidas Andrade.
*/
#include "Temperatura.h"
#include <string.h>
#include <dht11.h>

#define DHT11PIN 2

dht11 DHT11;
char floatValue[2];
char message[20];

void Temperatura::loopApp()
{
  if (BaseApp::_running)
  {
    int chk = DHT11.read(DHT11PIN);

    switch (chk)
    {
      case 0: Serial.println("OK"); break;
      case -1: Serial.println("Erro de Checksum"); break;
      case -2: Serial.println("Erro de Time Out"); break;
      default: Serial.println("Erro Desconhecido"); break;
    }

    dtostrf((float)DHT11.humidity, 2, 0, floatValue);
    _ledMatrix->printStringScroll(0, 0, "Umidade (%): ", MESSAGE_SPEED, '<');
    _ledMatrix->printStringScroll(0, 0, floatValue, MESSAGE_SPEED, '<');
    
    delay(500);

    dtostrf((float)DHT11.temperature, 2, 0, floatValue);
    _ledMatrix->printStringScroll(0, 0, "Temperatura (Celsius): ", MESSAGE_SPEED, '<');
    _ledMatrix->printStringScroll(0, 0, floatValue, MESSAGE_SPEED, '<');

    delay(2000);
  } 
}
