#include <SPI.h>
#include <Helios_LEDS.h>

Helios_LEDS led;

void setup()
{
  led.Init();
}

void loop()
{
  led.ClearAll();
  led.LedMatrixWrite("Hello World",1);
  delay(5000);
  led.LedMatrixScrollSpeed(20);
  while(led.LedMatrixBusy()){}
  led.SetRGB(0,1,0);
  delay(1000);
  char text[] = "Looping!!";
  led.SetRGB(1,1,0);
  led.LedMatrixLoop(text);
  delay(5000);
  led.LedMatrixClear();
  delay(1000);
  led.SetRGB(1,0,0);
  led.LedMatrixWrite("Max buffer length is 30 characters",16);
  while(led.LedMatrixBusy()){}
  led.SetRGB(0,0,0);
  led.SetLEDS(1,1,1);
  delay(1000);
}
  


