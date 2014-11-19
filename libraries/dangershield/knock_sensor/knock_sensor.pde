// Danger Board v1.0 Blank Script
// By: Zach Hoeken (hoeken@gmail.com)
// More info: make.nycresistor.com/ds-1.0

//our setup stuff
#include "_init.h"

void setup()
{
  ds_init();
}

int knock_value = 0;
int threshold = 50;

void loop()
{
  knock_value = analogRead(KNOCK_SENSOR_PIN);
  
  if (knock_value > threshold)
  {
    Serial.println(knock_value, DEC);
    Serial.println("knock");

    digitalWrite(LED1_PIN, HIGH);
    delay(100);
    digitalWrite(LED1_PIN, LOW);
  }
}
