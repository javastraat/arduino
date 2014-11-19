// Danger Board v1.0 LED Flashers
// By: Zach Hoeken (hoeken@gmail.com)
// More info: make.nycresistor.com/ds-1.0

//this is our setup stuff.
#include "_init.h"

void setup()
{
  ds_init();
}

void loop()
{
  //turn the leds on.
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, LOW);
  delay(500);
  
  //toggle the leds.
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, HIGH);
  delay(500);
}
