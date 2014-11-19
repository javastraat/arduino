// Danger Board v1.0 Blank Script
// By: Zach Hoeken (hoeken@gmail.com)
// More info: make.nycresistor.com/ds-1.0

#include "_init.h"
#include "_data.h"

void setup()
{
  ds_init();
}

void loop()
{
  for (int i=0; i<10; i++)
  {
    byte j = chardata[i];
    
    
    
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, j);
    digitalWrite(LATCH_PIN, HIGH);
  
    delay(250);
  }
}
