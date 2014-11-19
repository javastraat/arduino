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
long interval = 250;


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
    
    // clear the back-buffer
    GFX_Color_t background = Colorduino.color(0, 0, 0);
    Colorduino.fillColor(background);
    // create new color
    GFX_Color_t color = Colorduino.color(random(256), random(256), random(256));
    // draw pixel on the back-buffer
    Colorduino.drawLine(random(7), random(7), random(7), random(7), color);
    // swap the buffers, but don't copy the new front-buffer to the new back-buffer
    Colorduino.swapBuffers(false);
  }
}
