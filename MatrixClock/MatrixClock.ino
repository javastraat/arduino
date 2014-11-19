#include <Arduino.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <Wire.h>
#include "App.h"

// Buttonless 8x8 LED matrix clock.  Mark Wilson June 2012
void setup() 
{
  app.Init();
}

void loop() 
{
  app.Run();
}

