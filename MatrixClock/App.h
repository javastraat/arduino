#ifndef _APP_H
#define _APP_H
#include <Arduino.h>
#include "IMU.h"

class App
{
  public:
    void Init();
    void Run();
  
  private:
    void DoSplash(bool Reset);
    void SetMode(IMU::tOrientation Orientation);
    void CheckOrientation();
    void CheckBrightness();
    byte AmbientLight();
    bool IsConfigMode();
    enum tDisplayMode {eNone, e8x8Clock, eTxTClock, eTemperature, eMatrix, eDemo, eMarkov, eFaceUp, eFaceDown, eCfgAmbient, eCfgBrightness, eCfgHour, eCfgMinute};
    
    tDisplayMode m_DisplayMode;
    tDisplayMode m_LastConfigMode;
    IMU::tOrientation m_CurrentOrientation;
    unsigned long m_iOrientationTimerMS;
    
    // low pass filter
    byte m_iAmbientSampleCount;
    unsigned long m_iAmbientSamples;
    byte m_iLastAmbientLight;
    unsigned long m_iAmbientReadingTimer;
    
    byte m_iCFG_DimThreshold;
    byte m_iCFG_LowAmbientBrightness;
    byte m_iSelectedBrightness;
    byte m_iSelectedAmbientLight;
    byte m_iSelectedHour;
    byte m_iSelectedMinute;
};

extern App app;
#endif
