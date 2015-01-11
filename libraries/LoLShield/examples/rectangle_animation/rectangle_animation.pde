#include "Charliplexing.h"
#include "LOLDraw.h"


void setup()
{
  LedSign::Init();
}

void loop()
{
  char x1=4, y1=4, x2=9, y2= 4;
  for(x2; x2<=13; x2++)
  {
    LOLDraw::Rectangle(x1,y1,x2,y2);
    delay(100);
    
    LedSign::Clear();
    x1--;
    y1--;
    y2++;
  }
}
  

