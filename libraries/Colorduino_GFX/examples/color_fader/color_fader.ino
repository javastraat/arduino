/*
 * Fading example
 *
 */

// Include required libraries
#include <Adafruit_GFX.h>
#include <Colorduino_GFX.h>

// Create new Colorduino instance
ColorduinoPanel Colorduino;

// some vars required for the timing and ...
long previousMillis = 0;
long interval = 50;
// ... color fading
uint8_t red = 0, green = 0, blue = 255;
int8_t rd = 5, gd = 0, bd = 0;

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
    
    // calculate the next color
    red += rd;
    green += gd;
    blue += bd;

    if(bd < 0 && blue == 0) {
      bd = 0;
      gd = 5;
    }
    if(gd > 0 && green == 255) {
      gd = 0;
      rd = -5;
    }
    if(rd < 0 && red == 0) {
      rd = 0;
      bd = 5;
    }
    if(bd > 0 && blue == 255) {
      bd = 0;
      gd = -5;
    }
    if(gd < 5 && green == 0) {
      gd = 0;
      rd = 5;
    }
    if(rd > 0 && red == 255) {
      bd = -5;
      rd = 0;
    }

    // create new color
    GFX_Color_t color = Colorduino.color(red, green, blue);
    // fill back-buffer by using the given color
    Colorduino.fillColor(color);
    // swap the buffers, but don't copy the new front-buffer to the new back-buffer
    Colorduino.swapBuffers(false);
  }
}
