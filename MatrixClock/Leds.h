#ifndef _leds_h
#define _leds_h
#include <Arduino.h>

// 8x8 LED matrix
class LEDs
{
  public:
    enum tOrientation {eUp, eLeft, eRight, eDown};
    enum tBrightness {eOff, eLow, eHigh};
    void Init();
    void Mode(tOrientation Orientation);
    void Brightness(tBrightness Bright);
    tBrightness Brightness();
    void Clear();
    void Fill();
    void Set(int Row, int Col, bool On);
    void Show();
    byte* RawData();
    
private:
    static void Callback();
    void PortShiftOut(byte latchMask, byte dataMask, byte clockMask, byte Value);
    void Update();
    tBrightness m_iBrightness;
    tOrientation m_iOrientation;
    int m_iUpdateRow;
    int m_iUpdateCol;
    bool m_bDwellStart;
    int m_iDwellCounter;
    byte m_pPixelsA[8];           
    byte m_pPixelsB[8];
    byte* m_pPixels;
    byte* m_pNextPixels;

    static const int m_RowAnodePins[8];
    static const int m_ColCathodeBits[8];
};

extern LEDs leds;

#endif
