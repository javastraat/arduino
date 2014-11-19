/*
 * Animate a bouncing ball
 *
 */

// Include required libraries
#include <Adafruit_GFX.h>
#include <Colorduino_GFX.h>

// Set options

// radius 1-2
const int8_t r = 2;
// set to true to draw a filled circle
const boolean fillCircle = false;

// Create new Colorduino instance
ColorduinoPanel Colorduino;

// some vars required for timing and ...
long previousMillis = 0;
long interval = 250;

// ... movement
int8_t xSpeed = 1, ySpeed = 1;
int8_t x = 3, y = 4;

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
    
    x += xSpeed;
    y += ySpeed;
    
    if(x >= 7 - r || x <= r) {
      xSpeed = -xSpeed;
    }
    
    if(y >= 7 - r || y <= r) {
      ySpeed = -ySpeed;
    }

    // clear the back-buffer
    Colorduino.fillColor(Colorduino.color(0, 0, 0));
    // draw the circle
    if(fillCircle == false) {
      Colorduino.drawCircle(x, y, r, Colorduino.color(255, 255, 255));
    } else {
      Colorduino.fillCircle(x, y, r, Colorduino.color(255, 255, 255));
    }
    // swap the buffers, but don't copy the new front-buffer to the new back-buffer
    Colorduino.swapBuffers(false);
  }
}
