// Danger Board v1.0 Light Sensor
// By: Zach Hoeken (hoeken@gmail.com)
// More info: make.nycresistor.com/ds-1.0

//our setup stuff
#include "_init.h"

void setup()
{
  ds_init();
}

//our sensor value
int raw = 0;

void loop()
{
  
  //get a reading from our analog pin
  raw = analogRead(LIGHT_SENSOR_PIN);
  
  //now send it to the terminal
  Serial.print("Light Value: ");
  Serial.println(raw, DEC);
  
  //only send it once a second
  delay(1000);
}
