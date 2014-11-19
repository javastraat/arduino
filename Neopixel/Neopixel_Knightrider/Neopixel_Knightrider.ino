//
// Copyright 2013 Doug Diego
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUM_LIGHTS 4

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  knightRider();
}
 
void knightRider() {
    int loops = 2;
    int count = 0;
    int timer = 20;
  
    uint32_t low = strip.Color(0, 0, 0);
    uint32_t high = strip.Color(255,0,0);
  
    // Flash Lights
    for(int i = 0; i < loops; i++){
        for (count=0;count<NUM_LIGHTS;count++) {
            strip.setPixelColor(count, high);
            strip.show();
            delay(timer);
            strip.setPixelColor(count+1,high);
            strip.show();
            delay(timer);
            strip.setPixelColor(count, low);
            strip.show();
            delay(timer*2);
        }
        for (count=NUM_LIGHTS;count>0;count--) {
            strip.setPixelColor(count, high);
            strip.show();
            delay(timer);
            strip.setPixelColor(count-1, high);
            strip.show();
            delay(timer);
            strip.setPixelColor(count, low);
            strip.show();
            delay(timer*2);
        }
    }
    
    // Turn them off
    for( int i = 0; i<NUM_LIGHTS; i++){
        strip.setPixelColor(i, low);
        strip.show();
    }
}
