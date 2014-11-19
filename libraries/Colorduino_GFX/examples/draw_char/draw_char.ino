/*
 * Draw a character
 *
 */

// Include required libraries
#include <Adafruit_GFX.h>
#include <Colorduino_GFX.h>

// Create new Colorduino instance
ColorduinoPanel Colorduino;

// some vars required for timing and ...
long previousMillis = 0;
long interval = 250;

// ... char
unsigned char c = 0;

void setup() {
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
    Colorduino.fillColor(Colorduino.color(0, 0, 0));
    // draw a character
    Colorduino.drawChar(1, 0, c, Colorduino.color(255, 255, 255), Colorduino.color(0, 0, 0), 1);
    c++;
    // swap the buffers, but don't copy the new front-buffer to the new back-buffer
    Colorduino.swapBuffers(false);
  }
}
