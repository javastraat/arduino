//LedScroll Example: Print Hello World

#include "LedControl.h"
#include "LedScroll.h"

/*
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lCtrl=LedControl(12,11,10,1);
LedScroll lScroll = LedScroll(&lCtrl);

void setup() {  
  lCtrl.shutdown(0,false);  /* The MAX72XX is in power-saving mode -> wakeup call */
  lCtrl.setIntensity(0,8);  /* Set the brightness to a medium values */
  lCtrl.clearDisplay(0);    /* and clear the display */
  
  lScroll.setDelay(100);     /* LedScroll - set scroll speed (ms) */
}

void loop() { 
  lScroll.print("Hello World");
}
