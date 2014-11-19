#include <Wire.h>
#include <Helios_Temperature_Sensor_TMP006.h>

Helios_Temperature_Sensor_TMP006 tsensor;

char Temp[100];

void setup()
{
  Wire.begin();
  Serial.begin(9600);
}

void loop()
{
  unsigned int object = tsensor.ReadObject();
  unsigned int ambient = tsensor.ReadAmbient();
  sprintf(Temp,"De omgevings temperatuur = %d C De object temperatuur = %d C",ambient,object);
  Serial.println(Temp);
  delay(2000);
}


