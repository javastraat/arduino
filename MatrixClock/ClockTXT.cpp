#include <Arduino.h>
#include "ClockTXT.h"
#include "RTC.h"
#include "LEDS.h"
#include "MarkovWord.h"

ClockTXT clockTXT;

// PROGMEM arrays
#define PROGMEMSTR(_var, _str) prog_char _var[]  PROGMEM = _str;

void progmem_strcpy(char* pDst, const prog_char** pSrc, int Index)
{
  strcpy_P(pDst, (char*)pgm_read_word(&(pSrc[Index])));
}

void progmem_strcat(char* pDst, const prog_char** pSrc, int Index)
{
  strcat_P(pDst, (char*)pgm_read_word(&(pSrc[Index])));
}

size_t progmem_strlen(const prog_char** pSrc, int Index)
{
  return strlen_P((char*)pgm_read_word(&(pSrc[Index])));
}

//----------------------------------
PROGMEMSTR(_exactly, "O'Clock");PROGMEMSTR(_one, "one");PROGMEMSTR(_two, "two");PROGMEMSTR(_three, "three");PROGMEMSTR(_four, "four");PROGMEMSTR(_five, "five");PROGMEMSTR(_six, "six");PROGMEMSTR(_seven, "seven");PROGMEMSTR(_eight, "eight");PROGMEMSTR(_nine, "nine");
PROGMEMSTR(_ten, "ten");PROGMEMSTR(_eleven, "eleven");PROGMEMSTR(_twelve, "twelve");PROGMEMSTR(_thirteen, "thirteen");PROGMEMSTR(_fourteen, "fourteen");PROGMEMSTR(_fifteen, "fifteen");PROGMEMSTR(_sixteen, "sixteen");PROGMEMSTR(_seventeen, "seventeen");PROGMEMSTR(_eighteen, "eighteen");PROGMEMSTR(_nineteen, "nineteen");
PROGMEMSTR(_twenty, "twenty");PROGMEMSTR(_thirty, "thirty");PROGMEMSTR(_forty, "forty");PROGMEMSTR(_fifty, "fifty");

PGM_P clock_OrdinalNames[] PROGMEM =
{
  _exactly,  _one,  _two,  _three,  _four,  _five,  _six,  _seven,  _eight,  _nine,  
  _ten,  _eleven,  _twelve,  _thirteen,  _fourteen,  _fifteen,  _sixteen,  _seventeen,  _eighteen,  _nineteen,
  _twenty,  _thirty,  _forty,  _fifty,
};

void ClockTXT::Init()
{
  Start();
  m_bEnableEgg = true;
}

void ClockTXT::Start()
{
  m_iTimerMS = millis();
  m_iDisplayedHour = 255;
  m_iDisplayedMinute = 255;
  m_bScrollingHour = false;
  memset(m_pMatrixPixels, 0, 8);
  NextString(m_StringBuffer);
  m_bFirstWord = true;
}

void ClockTXT::NextString(char* Buffer)
{
  m_bScrollingHour = !m_bScrollingHour;

  if (m_bScrollingHour && rtc.ReadMinute() != m_iDisplayedMinute)
  {
    rtc.ReadTime();
    byte Hour = rtc.m_Hour24;
    byte Minute = rtc.m_Minute;
    if (Hour == 0) Hour = 12;
    if (Hour > 12) Hour -= 12;
    m_iDisplayedMinute = Minute;
    m_iDisplayedHour = Hour;
  }
  
  GetNumberName(Buffer, m_bScrollingHour?m_iDisplayedHour:m_iDisplayedMinute, !m_bScrollingHour);
  if (m_bScrollingHour)
  {
    AdjustCase(Buffer, false);
  }
  else
  {
    strcat(Buffer, "   ");  // a gap
  }
  if (m_iDisplayedMinute == 26 && m_iDisplayedHour == 6)
  {
    if (m_bEnableEgg)
    {
      strcpy(Buffer, ":)Hello Marisa!:)  ");  // Easter Egg! @ 6:26
      m_bEnableEgg = false;
      m_bScrollingHour = false;
      m_iDisplayedMinute = 255;
    }
  }
  else
  {
    m_bEnableEgg = true;
  }
  m_iTotalPixelCols = RenderString(Buffer);
  m_iCurrentPixelCol = 0;
}

void ClockTXT::NextWord(char* Buffer)
{
  CMarkovWord::Word(Buffer, 20, 5, 10);
  strcat(Buffer, "   ");  // a gap
  m_iTotalPixelCols = RenderString(Buffer);
  m_iCurrentPixelCol = 0;
}


void ClockTXT::UpdateTimeDisplay()
{
  unsigned long NowMS = millis();
  if ((NowMS - m_iTimerMS) < 75 && m_iDisplayedHour != 255)
  {
    return;
  }
  m_iTimerMS = NowMS;
  leds.Mode(LEDs::eDown);
  
  for (int Col = 0; Col < 8; Col++)
    for (int Row = 0; Row < 8; Row++)
      if (bitRead(m_PixelBuffer[m_iCurrentPixelCol + Col], Row))
        leds.Set(Row, Col, true);
      else
        leds.Set(Row, Col, false);
        
  m_iCurrentPixelCol++;
  if (m_iCurrentPixelCol > m_iTotalPixelCols)
  {
    NextString(m_WordBuffer);
  }
  leds.Show();
}

void ClockTXT::UpdateMarkovDisplay()
{
  unsigned long NowMS = millis();
  if ((NowMS - m_iTimerMS) < 75 && !m_bFirstWord)
  {
    return;
  }
  m_iTimerMS = NowMS;
  leds.Mode(LEDs::eDown);
  if (m_bFirstWord)
  {
    randomSeed(rtc.Seed());
    NextWord(m_WordBuffer);
    m_bFirstWord = false;
  }

  for (int Col = 0; Col < 8; Col++)
    for (int Row = 0; Row < 8; Row++)
      if (bitRead(m_PixelBuffer[m_iCurrentPixelCol + Col], Row))
        leds.Set(Row, Col, true);
      else
        leds.Set(Row, Col, false);
        
  m_iCurrentPixelCol++;
  if (m_iCurrentPixelCol > m_iTotalPixelCols)
  {
    NextWord(m_StringBuffer);
  }
  leds.Show();
}

void ClockTXT::UpdateMatrixDisplay()
{
  unsigned long NowMS = millis();
  if ((NowMS - m_iTimerMS) < 100)
  {
    return;
  }
  m_iTimerMS = NowMS;
  leds.Mode(LEDs::eRight);
  // shuffle
  for (int Row = 7; Row > 0; Row--)
    m_pMatrixPixels[Row] = m_pMatrixPixels[Row - 1];
  m_pMatrixPixels[0] = random();
  for (int Col = 0; Col < 8; Col++)
    for (int Row = 0; Row < 8; Row++)
      if (bitRead(m_pMatrixPixels[Row], Col))
        leds.Set(Row, Col, true);
      else
        leds.Set(Row, Col, false);
  leds.Show();
}

char* ClockTXT::AdjustCase(char* Str, bool Leading)
{
  if (Leading)
  {
    if (strlen(Str) > 0 && 'a' <= Str[0] && Str[0] <= 'z')
      Str[0] -= 32;
  }
  else
  {
    // full caps
    char* pStr = Str;
    while (*pStr)
    {
      if ('a' <= *pStr && *pStr <= 'z')
        *pStr -= 32;
      pStr++;
    }
  }
  
  return Str;
}

char* ClockTXT::GetNumberName(char* Buffer, byte Number, bool OhPrefix)
{
  char* Name = Buffer;
  strcpy(Name, "");
  if (0 < Number && Number < 60)
  {
    if (Number <= 20)
    {
      // 1..20 are in there in full
      if (1 <= Number && Number <= 9 && OhPrefix)
      {
        strcpy(Name, "oh-");
        progmem_strcat(Name, clock_OrdinalNames, Number);
      }
      else
      {
        progmem_strcpy(Name, clock_OrdinalNames, Number);
      }
    }
    else
    {
      // get the 20, 30, 40, 50 part
      progmem_strcpy(Name, clock_OrdinalNames, 20 + ((Number - 20) / 10));
      if (Number % 10 == 0)
      {
        // that's all there is
      }
      else
      {
        strcat(Name, "-");
        progmem_strcat(Name, clock_OrdinalNames, Number % 10);
      }
    }
  }
  else
  {
    progmem_strcpy(Name, clock_OrdinalNames, 0);
  }
  // max length is 13 chars
  return Name;
}

int ClockTXT::RenderString(char* Str)
{
  int PixelColumn = FONT5x7_WIDTH;
  char* pCh = Str;
  memset(m_PixelBuffer, 0, sizeof(m_PixelBuffer));
  while (*pCh)
  {
    int RowOffset = 0;
    bool Drop;
    bool Narrow;
    font5x7.Load(*pCh, Drop, Narrow);
    if (Drop)
      RowOffset++;
    for (int Col = 0; Col < FONT5x7_WIDTH; Col++)
      for (int Row = 0; Row < FONT5x7_HEIGHT; Row++)
        if (font5x7.GetPixel(Row, Col))
          bitWrite(m_PixelBuffer[PixelColumn + Col], RowOffset + Row, 1);
    pCh++;
    PixelColumn += (FONT5x7_WIDTH + 1);
    if (Narrow)
      PixelColumn--;
  }
  return PixelColumn;
}



