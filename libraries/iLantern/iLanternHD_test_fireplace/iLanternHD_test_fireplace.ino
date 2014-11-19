#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUM_LIGHTS 300

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {

  Serial.begin(9600); 
//  Bridge.begin();
  strip.begin();
  colorWipe(strip.Color(0, 0, 0), 0);  
  strip.show();



}


void loop() {

modeFireplace();

}

void modeFireplace(){
	uint8_t iRandom = 0;
	for(int iRow = 0; iRow<12; iRow++){

		for(int iCol = 0; iCol<25; iCol++){
                for(int iBlank = 0; iBlank < strip.numPixels(); iBlank++){strip.setPixelColor(iRow+(iCol*8), 0,0,0);}            
			iRandom = random(1,10);
			if (iRandom > 4){strip.setPixelColor(iCol, 220, 0, 0);}
			if (iRandom > 3){strip.setPixelColor(25+iCol, 255, 35, 0);}
			if (iRandom > 5){strip.setPixelColor(50+iCol, 255, 75, 0);}
			if (iRandom > 6){strip.setPixelColor(75+iCol, 255, 100, 0);}
			if (iRandom > 7){strip.setPixelColor(100+iCol, 255, 150, 0);}
			if (iRandom > 8){strip.setPixelColor(125+iCol, 255, 200, 0);}
			if (iRandom > 9){strip.setPixelColor(150+iCol, 255, 255, 255);}
//                  delay(1);
		}
	}
        strip.show();
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<300; i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


