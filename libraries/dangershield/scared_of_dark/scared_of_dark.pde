// Danger Board v1.0 Scared of Dark Script
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
  while (analogRead(LIGHT_SENSOR_PIN) < 400)
  {
    for (byte i=0; i<50; i++)
    {
      digitalWrite(BUZZER_PIN, HIGH);
      delayMicroseconds(700);
      digitalWrite(BUZZER_PIN, LOW);
      delayMicroseconds(700);
    }
  }
}  
