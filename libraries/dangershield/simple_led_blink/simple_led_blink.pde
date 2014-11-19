// Danger Board v1.0 Simple LED Blink
// By: Zach Hoeken (hoeken@gmail.com)
// More info: make.nycresistor.com/ds-1.0

//our setup stuff
#include "_init.h"

void setup()
{
  ds_init();
}

void loop()
{
  //turn LED on
  digitalWrite(LED1_PIN, HIGH);
  delay(500);

  //turn LED off
  digitalWrite(LED1_PIN, LOW);
  delay(500);
}
