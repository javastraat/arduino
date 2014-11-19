#include <Arduino.h>
#include "IMU.h"

IMU imu;

void IMU::Init(bool UseEXTERNAL, bool LowG)
{
  m_bUseEXTERNAL = UseEXTERNAL;  // 3.3V external vs 5.0V default analog reference
  m_bLowG = LowG;  // scaled to 1.5g vs 6g
  analogReference(m_bUseEXTERNAL?EXTERNAL:DEFAULT);
}

int IMU::ReadCentiG(int Pin)
{
  // signed 100ths of a G
  double Reading = ReadRaw(Pin);
  double MaxVoltage = m_bUseEXTERNAL?3.3:5.0;
  double VoltsPerG = m_bLowG?0.800:0.206;
  double Voltage = (MaxVoltage/2.0) - MaxVoltage*Reading/1024.0;
  return Voltage/VoltsPerG*100.0;
}

char* IMU::AsBar(int CentiG, char* Buffer, char Axis)
{
  int Reading = constrain(CentiG, -140, +140)/20;  // -7..+7
  char* pChr = Buffer;
  for (int Idx = 0; Idx <= 15; Idx++)
  {
    *pChr = ' ';
    pChr++;
  }
  *pChr = 0;
  Buffer[7] = Axis;
  if (Reading > 0)
  {
    for (int Idx = 1; Idx <= Reading; Idx++)
      Buffer[7 + Idx] = '>';
  }
  else
  {
    for (int Idx = 1; Idx <= abs(Reading); Idx++)
      Buffer[7 - Idx] = '<';
  }
  return Buffer;
}


int IMU::ReadRaw(int Pin)
{
  return analogRead(Pin);
}

#define BIAS_X 0
#define BIAS_Y 0
#define BIAS_Z 0

#define HIGH_G 55
#define LOW_G  45

IMU::tOrientation IMU::GetOrientation(int PinX, int PinY, int PinZ)
{
  int X = ReadCentiG(PinX) + BIAS_X;
  int Y = ReadCentiG(PinY) + BIAS_Y;
  int Z = ReadCentiG(PinZ) + BIAS_Z;
//Serial.print("X:"  );Serial.println(X);
//Serial.print("Y:"  );Serial.println(Y);
//Serial.print("Z:"  );Serial.println(Z);Serial.println();
  if (abs(X) < LOW_G && abs(Z) > HIGH_G)  // level in the X plane
  {
    if (Z > HIGH_G)  // upright in the Z plane
      return eUp;
    else
      return eDown;
  }
  else if (abs(Z) < LOW_G && abs(X) > HIGH_G)
  {
    if (X > HIGH_G)
      return eRight;
    else
      return eLeft;
  }
  else if (abs(Z) < LOW_G && abs(X) < LOW_G && abs(Y) > HIGH_G)
  {
    if (Y > HIGH_G)
      return eFaceDown;
    else
      return eFaceUp;
  }
  return eOther;
}


