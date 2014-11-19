#include <Wire.h>

#include <Helios_RTC_PCF8563.h>

#include <Helios.h>

Helios_RTC_PCF8563 rtc;

void setup()
{
  Wire.begin();
  rtc.SetAlarmDaily(12,22);
}

void loop()
{
  if(rtc.ReadAlarmFlag()){
    rtc.SetAlarmDaily(12,23);
    Helios.Off();
  }
}


