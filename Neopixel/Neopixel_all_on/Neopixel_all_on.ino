

#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUM_LIGHTS  4

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  allon();
}
 
void allon() {
  
    uint32_t low = strip.Color(0, 0, 0); 
    uint32_t high = strip.Color(255,80,0);
  
    // Turn them off
    for( int i = 0; i<NUM_LIGHTS; i++){
        strip.setPixelColor(i, high);
        strip.show();
    }   
}

