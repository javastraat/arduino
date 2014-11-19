// Danger Board v1.0 Simple Shift Register
// By: Zach Hoeken (hoeken@gmail.com)
// More info: make.nycresistor.com/ds-1.0

//our setup stuff
#include "_init.h"

void setup()
{
  ds_init();
}

void loop()
{
  //count up routine
  for (int j=0; j<256; j++)
  { 
    //ground latchPin and hold low for as long as you are transmitting
    digitalWrite(LATCH_PIN, 0);

	//send the actual data, least significant bit first.
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, j);
    
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(LATCH_PIN, 1);
    
	//show what we're shifting out
    Serial.println(j, DEC);
    
	//wait a bit.
	delay(250);
  }
}
