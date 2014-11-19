#ifndef _CLOCKTXT_H
#define _CLOCKTXT_H
#include "Font5x7.h"

class ClockTXT
{
  public:
    void Init();
    void Start();
    void UpdateTimeDisplay();
    void UpdateMatrixDisplay();
    void UpdateMarkovDisplay();
  private:
    char* GetNumberName(char* Buffer, byte Number, bool OhPrefix);
    char* AdjustCase(char* Str, bool Leading);
    int RenderString(char* Str);
    void NextString(char* Buffer);
    void NextWord(char* Buffer);
    byte m_iDisplayedHour;
    byte m_iDisplayedMinute;
    char m_StringBuffer[20];
    char m_WordBuffer[20];
    bool m_bFirstWord;
    byte m_PixelBuffer[20*(FONT5x7_WIDTH + 1)];
    bool m_bScrollingHour;
    bool m_bEnableEgg;
    int m_iTotalPixelCols;
    int m_iCurrentPixelCol;
    unsigned long m_iTimerMS;
    byte m_pMatrixPixels[8];           
};

extern ClockTXT clockTXT;
#endif
