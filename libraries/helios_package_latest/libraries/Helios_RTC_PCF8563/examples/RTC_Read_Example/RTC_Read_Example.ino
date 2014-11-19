#include <Helios_RTC_PCF8563.h>

#include <Wire.h>

Helios_RTC_PCF8563 rtc;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
}

void loop()
{
  Serial.println(rtc.ReadSmall());
  delay(1000);
}


