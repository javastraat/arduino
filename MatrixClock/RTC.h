#ifndef rtc_h
#define rtc_h

// Talk to the DS3232
class RTC
{
  public:
    RTC();
    void Init();
    byte BCD2Dec(byte BCD);
    byte Dec2BCD(byte Dec);
    void ReadTime(void);
    byte ReadMinute(void);
    byte ReadDayOfWeek(void);
    void WriteTime(void);
    byte ReadByte(byte Index);
    void WriteByte(byte Index, byte Value);
    
    int Seed();
    
    byte m_Hour24;      // 0..23
    byte m_Minute;      // 0..59
    byte m_Second;      // 0..59
    byte m_DayOfWeek;   // 1..7 
    byte m_DayOfMonth;  // 1..31
    byte m_Month;       // 1..12
    byte m_Year;        // 0..99
};

extern RTC rtc;


#endif
