/*
 * Draw random pixels
 *
 */

// Include required libraries
#include <Adafruit_GFX.h>
#include <Colorduino_GFX.h>

// Create new Colorduino instance
ColorduinoPanel Colorduino;

// some vars required for timing
long previousMillis = 0;
long interval = 50;


void setup() {
  // Initialize random generator
  randomSeed(analogRead(0));
  
  // Set port mode, load data structures and start the timer
  Colorduino.init();
  // Set white balance
  Colorduino.setWhiteBalance(36, 63, 63);
}

void loop() {
  // some timing stuff
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    
    // create new color
    GFX_Color_t color = Colorduino.color(random(256), random(256), random(256));
    // draw pixel on the back-buffer
    Colorduino.drawPixel(random(8), random(8), color);
    // swap the buffers and copy the new front-buffer to the new back-buffer
    Colorduino.swapBuffers(true);
  }
}
