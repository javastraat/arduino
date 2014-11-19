#include <Arduino.h>
#include "Leds.h"
#include "PINS.h"
#include "TimerOne.h"

#define PERIOD_MICROSECONDS_BRIGHT 100
#define PERIOD_MICROSECONDS_DIM    50
#define DWELL_BRIGHT 0
#define DWELL_DIM    300

LEDs leds;

// m_RowAnodePins[n] is the Arduino pin connected to the led matrix pin for row n+1
const int LEDs::m_RowAnodePins[8] = MATRIX_ANODE_ROW_PINS;

// m_ColCathodeBits[m] is the bit number on the '595 connected to the led matrix pin for col m+1
const int LEDs::m_ColCathodeBits[8] = MATRIX_CATHODE_COL_BITS;

void LEDs::Init()
{
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);
  
  // all off
  digitalWrite(PIN_LATCH, LOW);
  shiftOut(PIN_DATA, PIN_CLOCK, MSBFIRST, 0x00);  
  digitalWrite(PIN_LATCH, HIGH);
  for (int row = 0; row < 8; row++) 
  {
    pinMode(m_RowAnodePins[row], OUTPUT); 
    digitalWrite(m_RowAnodePins[row], HIGH);    
  } 
  m_iOrientation = eUp;
  m_iBrightness = eHigh;
  Clear();
  Show();
  m_iUpdateRow = 0;
  m_iUpdateCol = 0;
  m_iDwellCounter = 0;
  m_bDwellStart = false;
  m_pPixels = m_pPixelsA;
  m_pNextPixels = m_pPixelsB;
  Timer1.initialize(PERIOD_MICROSECONDS_BRIGHT);
  Timer1.attachInterrupt(LEDs::Callback);
}

void LEDs::Mode(tOrientation Orientation)
{
  noInterrupts();
  m_iOrientation = Orientation;
  interrupts();
}

LEDs::tBrightness LEDs::Brightness()
{
  return m_iBrightness;
}


void LEDs::Brightness(tBrightness Bright)
{
  m_iBrightness = Bright;
  noInterrupts();
  Timer1.setPeriod((m_iBrightness == eHigh)?PERIOD_MICROSECONDS_BRIGHT:PERIOD_MICROSECONDS_DIM);
  interrupts();
}

void LEDs::Clear()
{
  for (int Row = 0; Row < 8; Row++) 
    m_pNextPixels[Row] = 0x00;
}

void LEDs::Fill()
{
  for (int Row = 0; Row < 8; Row++) 
    m_pNextPixels[Row] = 0xFF;
}

void LEDs::Set(int Row, int Col, bool On)
{
  int Bit = On?1:0;
  switch (m_iOrientation)
  {
    case eDown:
    {
      // pin 1 top right
      bitWrite(m_pNextPixels[7-Row], 7-Col, Bit);
      break;
    }
    case eLeft:
    {
      // pin 1 bottom right
      bitWrite(m_pNextPixels[Col], 7-Row, Bit);
      break;
    }
    case eRight:
    {
      // pin 1 top left
      bitWrite(m_pNextPixels[7-Col], Row, Bit);
      break;
    }
    default:
    {
      // pin 1 bottom left
      bitWrite(m_pNextPixels[Row], Col, Bit);
      break;
    }
  }
}

void LEDs::Show()
{
  noInterrupts();
  byte* pSwap = m_pPixels;
  m_pPixels = m_pNextPixels;
  m_pNextPixels = pSwap;
  interrupts();
}

byte* LEDs::RawData()
{
  return m_pNextPixels;
}

void LEDs::Callback()
{
  leds.Update();
}

void LEDs::PortShiftOut(byte latchMask, byte dataMask, byte clockMask, byte Value)
{
  // we want our interrupt handler to be as fast as possible so a custom shiftOut.
  // the three pins must be on the same port
  byte Mask = 0x80;
  PORT_SHIFTOUT &= ~latchMask;
  
  //  MSBFIRST
  while (Mask)
  {
    if (Value & Mask)
      PORT_SHIFTOUT |=  dataMask;
    else
      PORT_SHIFTOUT &= ~dataMask;
    			
    PORT_SHIFTOUT |=  clockMask;
    PORT_SHIFTOUT &= ~clockMask;
    Mask >>= 1;
  }
  PORT_SHIFTOUT |= latchMask;
}

void LEDs::Update()
{
  if (m_iDwellCounter)
  {
    // idle to extend the off time and dim the display
    if (m_bDwellStart)
    {
      m_bDwellStart = false;
      // off:
      digitalWrite(m_RowAnodePins[m_iUpdateRow], HIGH);
      PortShiftOut(PORT_MASK_LATCH, PORT_MASK_DATA, PORT_MASK_CLOCK, 0x00);
    }
    m_iDwellCounter--;
    return;
  }
  
  // off:
  digitalWrite(m_RowAnodePins[m_iUpdateRow], HIGH);
  PortShiftOut(PORT_MASK_LATCH, PORT_MASK_DATA, PORT_MASK_CLOCK, 0x00);
  
  m_iUpdateRow++;
  if (m_iUpdateRow > 7)
  {
    m_iUpdateRow = 0;
    m_iUpdateCol++;
    if (m_iUpdateCol > 7)
    {
      m_iUpdateCol = 0;
      m_bDwellStart = true;
      m_iDwellCounter = (m_iBrightness == eHigh)?DWELL_BRIGHT:DWELL_DIM;
    }
  }
  
  if (bitRead(m_pPixels[m_iUpdateRow], m_iUpdateCol) && m_iBrightness != eOff)
  {
    // on:
    digitalWrite(m_RowAnodePins[m_iUpdateRow], LOW);    
    PortShiftOut(PORT_MASK_LATCH, PORT_MASK_DATA, PORT_MASK_CLOCK, bit(m_ColCathodeBits[m_iUpdateCol]));
  }
}

