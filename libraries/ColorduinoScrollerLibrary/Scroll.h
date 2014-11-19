/*
  Scroll.h-A library dependant on adafruits GFX library for colorduino. 
  Allows for easy scrolling with adjustable speed, colors and of course text.
*/
#ifndef Scroll_h
#define Scroll_h

#include "Arduino.h"

class Scroll
{
  public:

Scroll();

    Scroll(String text, int scrollspeed, int tr, int tg, int tb, int br, int bg, int bb, int displaynumber);

    
  
};

#endif