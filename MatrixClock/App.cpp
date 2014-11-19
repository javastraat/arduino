#include <Arduino.h>
#include "App.h"
#include "PINS.h"
#include "RTC.h"
#include "LEDs.h"
#include "IMU.h"
#include "Clock8x8.h"
#include "ClockTXT.h"

App app;

#define CFG_AMBIENT_IDX 0x14
#define CFG_BRIGHT_IDX  0x15

void App::Init()
{
  Serial.begin(38400);
  pinMode(PIN_LED, OUTPUT);
  font5x7.Init();
  rtc.Init();
  leds.Init();
  imu.Init(true, true);
  IMU::tOrientation Orientation = imu.GetOrientation(PIN_IMU_X, PIN_IMU_Y, PIN_IMU_Z);
  m_iOrientationTimerMS = millis();
  m_iAmbientReadingTimer = millis();
  m_iAmbientSampleCount = 0;
  m_iLastAmbientLight = 255;
  m_CurrentOrientation = IMU::eOther;
  m_LastConfigMode = eNone;

  m_iCFG_DimThreshold = rtc.ReadByte(CFG_AMBIENT_IDX);
  constrain(m_iCFG_DimThreshold, 0, 99);
  m_iCFG_LowAmbientBrightness = (LEDs::tBrightness)rtc.ReadByte(CFG_BRIGHT_IDX);
  if (m_iCFG_LowAmbientBrightness > LEDs::eHigh)
    m_iCFG_LowAmbientBrightness = LEDs::eLow;

  if (Orientation == IMU::eFaceDown)
  {
    // power-reset
    m_iCFG_DimThreshold = 99;
    m_iCFG_LowAmbientBrightness = LEDs::eHigh;
    rtc.WriteByte(CFG_AMBIENT_IDX, m_iCFG_DimThreshold);
    rtc.WriteByte(CFG_BRIGHT_IDX, m_iCFG_LowAmbientBrightness);
  }
  SetMode(Orientation);
  clock8x8.Init();
  clockTXT.Init();
  DoSplash(Orientation == IMU::eFaceDown);
}

void App::Run()
{
  CheckOrientation();
  CheckBrightness();
  switch (m_DisplayMode)
  {
    case e8x8Clock:
    {
      clock8x8.UpdateTimeDisplay();
      break;
    }
    case eTxTClock:
    {
      clockTXT.UpdateTimeDisplay();
      break;
    }
    case eTemperature:
    {
      clock8x8.UpdateTemperatureDisplay();
      break;
    }
    case eMatrix:
    {
      clockTXT.UpdateMatrixDisplay();
      break;
    }
    case eDemo:
    {
      clock8x8.UpdateDemoDisplay();
      break;
    }
    case eMarkov:
    {
      clockTXT.UpdateMarkovDisplay();
      break;
    }
    case eFaceUp:
    case eFaceDown:
    {
      break;
    }
    case eCfgAmbient:
    {
      if (clock8x8.UpdateAmbientDisplay(m_iSelectedAmbientLight))
      {
        m_iSelectedAmbientLight = m_iLastAmbientLight;
      }
      break;
    }
    case eCfgBrightness:
    {
      clock8x8.UpdateBrightnessSetting(m_iSelectedBrightness);
      break;
    }
    case eCfgHour:
    {
      clock8x8.UpdateHourDisplay(m_iSelectedHour, m_iSelectedMinute);
      break;
    }
    case eCfgMinute:
    {
      clock8x8.UpdateMinuteDisplay(m_iSelectedMinute, m_iSelectedHour);
      break;
    }
  }
}

byte kSplash[] = 
{
  B11111111,
  B10000001,
  B10000001,
  B10000001,
  
  B00000000,
  B01111110,
  B01000010,
  B01000010,

  B00000000,
  B00000000,
  B00111100,
  B00100100,

  B00000000,
  B00000000,
  B00000000,
  B00011000,
};

byte kMW[] =
{
  B00010001,
  B00011011,
  B00010101,
  B00010001,
  B10001000,
  B10101000,
  B11011000,
  B10001000,
};

void DoubleDown(byte* pHalfImage)
{
  byte* pRaw = leds.RawData();
  for (int Idx = 0; Idx < 4; Idx++)
  {
    pRaw[Idx] = pHalfImage[Idx];
    pRaw[7 - Idx] = pHalfImage[Idx];
  }
}

void App::DoSplash(bool Reset)
{
  leds.Mode(LEDs::eUp);

  if (Reset)
  {
    leds.Clear();
    clock8x8.RenderGlyph(IDX_R);
    delay(5000);
  }
  memcpy(leds.RawData(), kMW, 8);
  leds.Show();
  delay(1000);
  
  byte* pSplash = kSplash;
  int Frame;
  for (Frame = 0; Frame < 4; Frame++)
  {
    DoubleDown(pSplash);
    pSplash += 4;
    leds.Show();
    delay(250);
  }
  pSplash -= 4;
  for (Frame = 0; Frame < 3; Frame++)
  {
    pSplash -= 4;
    DoubleDown(pSplash);
    leds.Show();
    delay(250);
  }
  leds.Fill();
  leds.Show();
  delay(500);
}

/*
   +--.
   |  | 8x8 clock
   +--+

   .--+
   |  | Temperature
   +--+

   +--+
   |  | Txt clock
   .--+

   +--+
   |  | "Matrix"
   +--.
   
   +--. (face UP)
   |  |
   |  |        +--+                        +--.
   |  |     -> |  | set Ambient; Bright -> |  | (set)
   +--+        +--.                        +--+
   
               .--+                        +--.
            -> |  | set Hour; Minute    -> |  | (set)
               +--+                        +--+


               +--+
            -> |  | Markov random words
               .--+
               
   +--+ (face DOWN)
   |  |
   |  |        +--+
   |  |     -> |  | 8x8 fast clock demo
   +--.        .--+
   
            -> [power on] reset ambient (99) & bright (hi)

*/
void App::SetMode(IMU::tOrientation Orientation)
{
  if (m_CurrentOrientation == Orientation)
    return;
    
  switch (Orientation)
  {
    case IMU::eUp:
      clock8x8.Start();    
      // "home" position, save config?
      if (m_DisplayMode == eCfgAmbient)
      {
        // record the ambient level
        m_iCFG_DimThreshold = m_iSelectedAmbientLight;
        rtc.WriteByte(CFG_AMBIENT_IDX, m_iCFG_DimThreshold);
      }
      else if (m_DisplayMode == eCfgBrightness)
      {
        // record the brightness
        m_iCFG_LowAmbientBrightness = m_iSelectedBrightness;
        rtc.WriteByte(CFG_BRIGHT_IDX, m_iCFG_LowAmbientBrightness);
      }
      else if (m_DisplayMode == eCfgHour)
      {
        // set the hour
        rtc.m_Hour24 = m_iSelectedHour;
        rtc.WriteTime();
      }
      else if (m_DisplayMode == eCfgMinute)
      {
        // set the minute
        rtc.m_Minute = m_iSelectedMinute;
        rtc.WriteTime();
      }
      m_DisplayMode = e8x8Clock;
      break;
    case IMU::eDown:
      clockTXT.Start();
      if (m_DisplayMode == eFaceDown)
      {
        m_DisplayMode = eDemo;
      }
      else if (m_DisplayMode == eFaceUp)
      {
        m_DisplayMode = eMarkov;
      }
      else
      {
        m_DisplayMode = eTxTClock;
      }
      break;
    case IMU::eLeft:
      clock8x8.Start();
      if (m_DisplayMode == eFaceUp)
      {
        // configure time
        rtc.ReadTime();
        m_iSelectedHour = rtc.m_Hour24;
        if (m_iSelectedHour == 0) m_iSelectedHour = 12;
        if (m_iSelectedHour > 12) m_iSelectedHour -= 12;
         m_iSelectedMinute = rtc.m_Minute;
        if (m_LastConfigMode != eCfgHour)   
        {
          m_DisplayMode = eCfgHour;
        }
        else
        {
          m_DisplayMode = eCfgMinute;
        }
      }
      else
      {
        m_DisplayMode = eTemperature;
      }
      break;
    case IMU::eRight:
      if (m_DisplayMode == eFaceUp)
      {
        // config brightness
        clock8x8.Start();
        if (m_LastConfigMode != eCfgAmbient)   
        {
          m_iSelectedAmbientLight = m_iCFG_DimThreshold;
          m_DisplayMode = eCfgAmbient;
        }
        else
        {
          m_iSelectedBrightness = m_iCFG_LowAmbientBrightness;
          m_DisplayMode = eCfgBrightness;
        }
      }
      else
      {
        clockTXT.Start();    
        m_DisplayMode = eMatrix;
      }
      break;
    case IMU::eFaceUp:
      // MW
      m_DisplayMode = eFaceUp;
      leds.Mode(LEDs::eUp);
      memcpy(leds.RawData(), kMW, 8);
      leds.Show();
      break;
    case IMU::eFaceDown:
      // blank
      m_DisplayMode = eFaceDown;
      leds.Clear();
      leds.Show();
      break;
    case IMU::eOther:
      return;
  }
  if (IsConfigMode())
  {
    m_LastConfigMode = m_DisplayMode;
  }
  m_CurrentOrientation = Orientation;
}

void App::CheckOrientation()
{
  // check periodically to see if the orientation has changed
  unsigned long NowMS = millis();
  if ((NowMS - m_iOrientationTimerMS) < 250)
  {
    return;
  }
  m_iOrientationTimerMS = NowMS;
  SetMode(imu.GetOrientation(PIN_IMU_X, PIN_IMU_Y, PIN_IMU_Z));
}

void App::CheckBrightness()
{
  LEDs::tBrightness Bright;
  if (AmbientLight() > m_iCFG_DimThreshold || IsConfigMode())
  {
    Bright = LEDs::eHigh;
  }
  else
  {
    Bright = (LEDs::tBrightness)m_iCFG_LowAmbientBrightness;
  }
  if (Bright != leds.Brightness())
    leds.Brightness(Bright);
}

bool App::IsConfigMode()
{
 return (m_DisplayMode >= eCfgAmbient);
}

byte App::AmbientLight()
{
  // check periodically to see if the ambient light has changed and if we should alter the brightness
  unsigned long NowMS = millis();
  if ((NowMS - m_iAmbientReadingTimer) < 1000)
  {
    return m_iLastAmbientLight;
  }
  m_iAmbientReadingTimer = NowMS;
  // ambient light reading from LDR: 
  byte Ambient = analogRead(PIN_LDR);
  constrain(Ambient, 0, 99);
  // low-pass filter (moving avg)
  int Sum = Ambient;
  unsigned long Samples = m_iAmbientSamples;
  for (int Sample = 0; Sample < m_iAmbientSampleCount; Sample++)
  {
    Sum += (Samples & 0xFF);
    Samples >>= 8;  // store 4 sample bytes in an unsigned long
  }
  m_iAmbientSamples <<= 8;
  m_iAmbientSamples |= Ambient;
  Ambient = Sum / (m_iAmbientSampleCount + 1);
  m_iAmbientSampleCount = min(m_iAmbientSampleCount + 1, sizeof(m_iAmbientSamples));
  
  m_iLastAmbientLight = Ambient;
  return m_iLastAmbientLight;
}

