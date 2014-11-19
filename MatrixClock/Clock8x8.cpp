#include <Arduino.h>
#include "Clock8x8.h"
#include "RTC.h"
#include "LEDS.h"

Clock8x8 clock8x8;

#define CONFIG_BLINK_TIME 500
#define CONFIG_INC_COUNTER 5

word Clock8x8::kFont4x4[] =
{
  0x7557, // 0
  0x6227,
  0x6127,
  0x7317,
  0x4571,

  0x7616, // 5
  0x4757,
  0x7111,
  0xEA57, // "wide" 8
  0x7571,

  0xADDA, // 10
  0x5555, // 11
  0xB9AB, // 12
  
  0x7757, // narrow 8
  0x0525, // x
  
  0x6565,  // R (eset)

  0x9DB9,  // N
  0x7557,  // O
  0x888F,  // L
  0x9F99,  // H
  0x7227,  // I
};

byte Clock8x8::kMinutes2[] =
{
    // 0x<Col><Row> row, col of minutes LEDs around perimeter. hi bit means corner
    0x40, 
    0x50,
    0x60,
     0x70 | 0x80,
    0x71,
    0x72,
    0x73,
    0x74,
    0x75,
    0x76,
     0x77 | 0x80,
    0x67,
    0x57,
    0x47,
    0x37,
    0x27,
    0x17,
     0x07 | 0x80,
    0x06,
    0x05,
    0x04,
    0x03,
    0x02,
    0x01,
     0x00 | 0x80,
    0x10,
    0x20,
    0x30
};

byte kQuarters[] =
{
    // 0x<Col><Row> row, col of minutes LEDs around perimeter. hi bit means start
  
    0x30 | 0x80,
    0x40, 
    0x73 | 0x80,
    0x74,
    0x47 | 0x80,
    0x37,
    0x04 | 0x80,
    0x03,
    0x00 | 0x80,
};

void Clock8x8::Init()
{
  Start();
  m_iHourStyle = eNoHour;
  m_iMinuteStyle = eFivesAndSolidQuarters;
}

void Clock8x8::Start()
{
  m_iTimerMS = millis();
  m_iDisplayedHour = 255;
  m_iDisplayedMinute = 255;
  m_iDisplayedTemperature = 255;  
  m_bStart = true;
  m_bConfigBlinkOn = true;
  m_iConfigBlinkInterval = CONFIG_BLINK_TIME/2;      // faster at the start
  m_iConfigIncrementCounter = CONFIG_INC_COUNTER*2;  // longer at the start
}

void Clock8x8::UpdateTimeDisplay()
{
  unsigned long NowMS = millis();
  if ((NowMS - m_iTimerMS) < 30000 && m_iDisplayedHour != 255)
  {
    return;
  }
  m_iTimerMS = NowMS;

  if (rtc.ReadMinute() == m_iDisplayedMinute)
  {
    return;
  }
  rtc.ReadTime();
  byte Hour = rtc.m_Hour24;
  byte Minute = rtc.m_Minute;
  if (Hour == 0) Hour = 12;
  if (Hour > 12) Hour -= 12;
  m_iDisplayedMinute = Minute;
  m_iDisplayedHour = Hour;
  
  leds.Mode(LEDs::eUp);
  leds.Clear();
  RenderHour(Hour);
  RenderMinute(Minute, m_iHourStyle, m_iMinuteStyle);
  leds.Show();
}
  

void Clock8x8::UpdateDemoDisplay()
{
  unsigned long NowMS = millis();
  if ((NowMS - m_iTimerMS) < 100 && m_iDisplayedHour != 255)
  {
    return;
  }
  m_iTimerMS = NowMS;

  if (m_iDisplayedHour == 255)
  {
    m_iDisplayedHour = 1;
    m_iDisplayedMinute = 0;
  }
  m_iDisplayedMinute++;
  if (m_iDisplayedMinute > 59)
  {
    m_iDisplayedMinute = 0;
    m_iDisplayedHour++;
    if (m_iDisplayedHour > 12)
      m_iDisplayedHour = 1;
  }
  leds.Mode(LEDs::eDown);
  leds.Clear();
  RenderHour(m_iDisplayedHour);
  RenderMinute(m_iDisplayedMinute, m_iHourStyle, m_iMinuteStyle);
  leds.Show();
}
  
void Clock8x8::RenderMinute(byte Minute, tHourStyle HourStyle, tMinuteStyle MinuteStyle)
{
  if (MinuteStyle != eQuartersOnly)
  {
    int Solid = -1;  // show 15, 30, 45 as solid, corners too
    if (MinuteStyle == eFivesAndSolidQuarters)
    {
      if (Minute >= 45) 
        Solid = 45;
      else if (Minute >= 30) 
        Solid = 30;
      else if (Minute >= 15) 
        Solid = 15;
    }
    int Minutes = Minute*2;
    Solid *= 2;
    int Check = 0;
    for (int Idx = 0; Idx < 28; Idx++)
    {
      int cnr =  kMinutes2[Idx] & 0x80;
      int row =  kMinutes2[Idx] & 0x07;
      int col = (kMinutes2[Idx] & 0x70) >> 4;
      if (!cnr)
      {
        Check += 5;
      }
      if ((Check <= Minutes) && (Check % 10 == 0  || Check < Solid))
        leds.Set(row, col, true);
    }
    if (Minutes < 10)
    {
      // special for on the hour
      if (HourStyle == eCornersHour)
      {
        leds.Set(0, 7, true);
        leds.Set(7, 0, true);
        leds.Set(0, 0, true);
        leds.Set(7, 7, true);
      } 
      else if (HourStyle == eTopHour)
      {
        leds.Set(0, 3, true);
        leds.Set(0, 4, true);
      }
    }
  }
  else
  {
    int Qtr = (Minute / 15) + 1;
    byte* pLed = kQuarters;
    while (Qtr)
    {
      if (*pLed & 0x80)
      {
        Qtr--;
        if (!Qtr)
        {
          do
          {
            leds.Set(*pLed & 0x07, (*pLed & 0x70) >> 4, true);
            pLed++;
          } while (!(*pLed & 0x80));
          break;
        }
      }
      pLed++;
    }
  }
}

void Clock8x8::RenderHour(byte Hour)
{
  unsigned short int Mask = 0x8000;
  unsigned short int Defn = kFont4x4[Hour];
  for (int r = 2; r < 6; r++)
    for (int c = 2; c < 6; c++)
    {
      leds.Set(r, c, (Mask & Defn) != 0);
      Mask >>= 1;
    }
}

void Clock8x8::UpdateTemperatureDisplay()
{
  unsigned long NowMS = millis();
  if ((NowMS - m_iTimerMS) < 30000 && m_iDisplayedTemperature != 255)
  {
    return;
  }
  m_iTimerMS = NowMS;

  byte Temp = rtc.ReadByte(0x11);
  if (Temp == m_iDisplayedTemperature)
  {
    return;
  }
  m_iDisplayedTemperature = Temp;
  leds.Clear();
  leds.Mode(LEDs::eLeft);
  RenderNumber(Temp);
}

bool Clock8x8::UpdateAmbientDisplay(byte Light)
{
  unsigned long NowMS = millis();
  if ((NowMS - m_iTimerMS) < m_iConfigBlinkInterval && !m_bStart)
  {
    return false;
  }
  m_iTimerMS = NowMS;
  
  leds.Clear();
  leds.Mode(LEDs::eRight);
  Light = constrain(Light, 0, 99);
  if (m_bConfigBlinkOn)
    RenderNumber(Light);
  else
    leds.Show();
  m_bStart = false;
  m_bConfigBlinkOn = !m_bConfigBlinkOn;
  if (!m_bConfigBlinkOn)
  {
    m_iConfigIncrementCounter--;
    if (m_iConfigIncrementCounter <= 0)
    {
      m_iConfigBlinkInterval = CONFIG_BLINK_TIME;
      m_iConfigIncrementCounter = CONFIG_INC_COUNTER;
      return true;  // ask for an update
    }
  }
  return false;
}

void Clock8x8::UpdateBrightnessSetting(byte& Brightness)
{
  unsigned long NowMS = millis();
  if ((NowMS - m_iTimerMS) < m_iConfigBlinkInterval && !m_bStart)
  {
    return;
  }
  m_iTimerMS = NowMS;
  
  leds.Clear();
  byte Idx1 = IDX_x;
  byte Idx2 = IDX_x;
  switch (Brightness)
  {
    case LEDs::eOff:
      Idx1 = IDX_N;
      Idx2 = IDX_O;
      break;
    case LEDs::eLow:
      Idx1 = IDX_L;
      Idx2 = IDX_O;
      break;
    case LEDs::eHigh:
      Idx1 = IDX_H;
      Idx2 = IDX_I;
      break;
  }
  leds.Mode(LEDs::eRight);
  if (m_bConfigBlinkOn)
  {
    RenderGlyph(Idx1, 0, false);
    RenderGlyph(Idx2, 4, true);
  }
  else
    leds.Show();
  m_bStart = false;
  m_bConfigBlinkOn = !m_bConfigBlinkOn;
  if (!m_bConfigBlinkOn)
  {
    m_iConfigIncrementCounter--;
    if (m_iConfigIncrementCounter <= 0)
    {
      m_iConfigBlinkInterval = CONFIG_BLINK_TIME;
      m_iConfigIncrementCounter = CONFIG_INC_COUNTER;
      Brightness++;
      if (Brightness > LEDs::eHigh)
        Brightness = LEDs::eOff;
    }
  }
}

void Clock8x8::UpdateHourDisplay(byte& Hour, byte Minute)
{
  unsigned long NowMS = millis();
  if ((NowMS - m_iTimerMS) < m_iConfigBlinkInterval && !m_bStart)
  {
    return;
  }
  m_iTimerMS = NowMS;
  
  leds.Clear();
  leds.Mode(LEDs::eLeft);
  RenderMinute(Minute, m_iHourStyle, m_iMinuteStyle);
  if (m_bConfigBlinkOn)
    RenderHour(Hour);
  leds.Show();
  m_bStart = false;
  m_bConfigBlinkOn = !m_bConfigBlinkOn;
  if (!m_bConfigBlinkOn)
  {
    m_iConfigIncrementCounter--;
    if (m_iConfigIncrementCounter <= 0)
    {
      m_iConfigBlinkInterval = CONFIG_BLINK_TIME;
      m_iConfigIncrementCounter = CONFIG_INC_COUNTER;
      Hour++;
      if (Hour > 12)
        Hour = 1;
    }
  }
}

void Clock8x8::UpdateMinuteDisplay(byte& Minute, byte Hour)
{
  unsigned long NowMS = millis();
  if ((NowMS - m_iTimerMS) < m_iConfigBlinkInterval && !m_bStart)
  {
    return;
  }
  m_iTimerMS = NowMS;
  
  leds.Clear();
  leds.Mode(LEDs::eLeft);
  RenderHour(Hour);
  if (m_bConfigBlinkOn)
    RenderMinute(Minute, eTopHour, m_iMinuteStyle);  // over-ride style to not be blinking "nothing"
  leds.Show();
  m_bStart = false;
  m_bConfigBlinkOn = !m_bConfigBlinkOn;
  if (!m_bConfigBlinkOn)
  {
    m_iConfigIncrementCounter--;
    if (m_iConfigIncrementCounter <= 0)
    {
      m_iConfigBlinkInterval = CONFIG_BLINK_TIME;
      m_iConfigIncrementCounter = CONFIG_INC_COUNTER;
      Minute += 5;
      if (Minute > 59)
        Minute = 0;
    }
  }
}

void Clock8x8::RenderNumber(byte Num)
{
  int Idx;
  if (Num > 9)
  {
    if (Num <= 99)
      Idx = Num / 10;
    else
      Idx = IDX_x;
    if (Idx == 8) 
      Idx = IDX_NARROW_8;
    RenderGlyph(Idx, 0, false);
  }
  if (Num <= 99)
    Idx = Num % 10;
  else
    Idx = IDX_x;
  if (Idx == 8) 
    Idx = IDX_NARROW_8;
  RenderGlyph(Idx, 4, false);
  leds.Show();
}

void Clock8x8::RenderGlyph(byte Idx, byte ColOffset, bool Show)
{
  unsigned short int Mask = 0x8000;
  unsigned short int Defn = kFont4x4[Idx];
  for (int r = 2; r < 6; r++)
    for (int c = 0; c < 4; c++)
    {
      leds.Set(r, c + ColOffset, (Mask & Defn) != 0);
      Mask >>= 1;
    }
  
  if (Show)
    leds.Show();
}

