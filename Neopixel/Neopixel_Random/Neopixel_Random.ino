#include <Adafruit_NeoPixel.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(4, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  randomWipe();
  //noise
}

void randomWipe() {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    int r = rand(256);
    int g = rand(256);
    int b = rand(256);
    uint32_t c = strip.Color(r,g,b);
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(100);
}




void noise() {
  i = rand(strip.numPixels());
  int r  = rand(256);
  int g = rand(256);
  int b = rand(256);
  uint32_t c = strip.Color(r,g,b);
  strip.setPixelColor(i, c);
  strip.show();
  delay(5);
}
