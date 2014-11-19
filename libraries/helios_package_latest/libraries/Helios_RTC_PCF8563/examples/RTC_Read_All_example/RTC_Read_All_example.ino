#include <SPI.h>
#include <Helios_LEDS.h>
#include <Wire.h>
#include <Helios_RTC_PCF8563.h>
#include <Helios.h>

Helios_RTC_PCF8563 rtc;
Helios_LEDS led;

void setup()
{
  led.Init();
  Wire.begin();
  rtc.SetAll(12,31,40,5,24,5,13);
}

void loop()
{
  char *time = rtc.ReadAll();
  led.LedMatrixWrite(time,15);
  delay(1000);
}


