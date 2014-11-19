#ifndef _IMU_H
#define _IMU_H

class IMU
{
  public:
    enum tOrientation {eOther, eUp, eLeft, eRight, eDown, eFaceUp, eFaceDown};
  
    void Init(bool UseEXTERNAL, bool LowG);
    int ReadCentiG(int Pin);
    int ReadRaw(int Pin);
    char* AsBar(int CentiG, char* Buffer, char Axis);
    
    tOrientation GetOrientation(int PinX, int PinY, int PinZ);
    
  private:
    bool m_bUseEXTERNAL;
    bool m_bLowG;
};

extern IMU imu;

#endif
