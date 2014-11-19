#ifndef _FONT5x5_H
#define _FONT5x7_H

#define FONT5x7_HEIGHT 7
#define FONT5x7_WIDTH  5

class Font5x7
{
  public:
    void Init();
    bool Load(char Char, bool& Drop, bool& Narrow);
    bool GetPixel(int Row, int Col);
  private:
    byte m_Defn[8];
};

extern Font5x7 font5x7;

#endif
