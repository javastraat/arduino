#ifndef _clock8x8_h
#define _clock8x8_h

#define IDX_NARROW_8 13
#define IDX_x        14
#define IDX_R        15
#define IDX_N        16
#define IDX_O        17
#define IDX_L        18
#define IDX_H        19
#define IDX_I        20

class Clock8x8
{
public:
  enum tHourStyle {eNoHour, eTopHour, eCornersHour};
  enum tMinuteStyle {eFivesOnly, eFivesAndSolidQuarters, eQuartersOnly};
  
  void Init();
  void Start();
  void UpdateTimeDisplay();
  void UpdateDemoDisplay();
  void UpdateTemperatureDisplay();
  bool UpdateAmbientDisplay(byte Light);
  void UpdateBrightnessSetting(byte& Brighness);
  void UpdateHourDisplay(byte& Hour, byte Minute);
  void UpdateMinuteDisplay(byte& Minute, byte Hour);
  void RenderGlyph(byte Idx, byte ColOffset = 2, bool Show = true);

private:
  void RenderHour(byte Hour);
  void RenderMinute(byte Minute, tHourStyle HourStyle, tMinuteStyle MinuteStyle);
  void RenderNumber(byte Num);
  tHourStyle m_iHourStyle;
  tMinuteStyle m_iMinuteStyle;
  byte m_iDisplayedHour;
  byte m_iDisplayedMinute;
  byte m_iDisplayedTemperature;  
  unsigned long m_iTimerMS;
  bool m_bStart;
  bool m_bConfigBlinkOn;
  int  m_iConfigBlinkInterval;
  int  m_iConfigIncrementCounter;
  
  static word kFont4x4[];
  static byte kMinutes2[];
};

extern Clock8x8 clock8x8;

#endif
