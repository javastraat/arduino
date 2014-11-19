

#include <Adafruit_NeoPixel.h>
#include <colorcodes.h>

#define PIN 6
#define NUM_LIGHTS  4

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
// NEO_RGB Pixels are wired for RGB bitstream
// NEO_GRB Pixels are wired for GRB bitstream
// NEO_KHZ400 400 KHz bitstream (e.g. FLORA pixels)
// NEO_KHZ800 800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
strip.begin();
strip.show(); // Initialize all pixels to 'off'
}

void loop() {
// Some example procedures showing how to display to the pixels:
 {
colorWipe(strip.Color(red), 100); // Red
colorWipe(strip.Color(0, 0, 0), 0);

}}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
for(uint16_t i=0; i<strip.numPixels(); i++) {
strip.setPixelColor(i, c);
strip.show();
delay(wait);
}
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
if(WheelPos < 85) {
return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
} else if(WheelPos < 170) {
WheelPos -= 85;
return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
} else {
WheelPos -= 170;
return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
}
}
