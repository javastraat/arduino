#include <Wire.h>
#include <Helios_Temperature_Sensor_TMP006.h>
#include <SPI.h>
#include <Helios_LEDS.h>

Helios_LEDS led;

Helios_Temperature_Sensor_TMP006 tsensor;
char Data1[30];
char Temp[100];

void setup()
{
  Wire.begin();
  Serial.begin(9600);
    led.Init();
}

void loop()
{
  unsigned int object = tsensor.ReadObject();
  unsigned int ambient = tsensor.ReadAmbient();
  unsigned int temp = tsensor.ReadAmbient();
  sprintf(Temp,"De processor temperatuur = %d C De lucht temperatuur = %d C",ambient,object);
  Serial.println(temp);
  
  led.LedMatrixWrite(temp,70);
  while(led.LedMatrixBusy()){}
}


