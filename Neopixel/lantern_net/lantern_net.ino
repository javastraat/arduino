#include <FileIO.h>
#include <SPI.h>
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>

#define PIN 6
#define NUM_LIGHTS 56
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 7, PIN,
  NEO_MATRIX_BOTTOM  + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS    + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

char colorbuffer[256]; 
String color;
YunServer server;

const uint16_t matrixColors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

int x    = matrix.width();
int pass = 0;

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(matrixColors[0]);
//  matrix.setTextSize(1);

  server.listenOnLocalhost();
  server.begin();
  Serial.begin(9600); 

  Bridge.begin(); 	/* Initialize the Bridge */
  FileSystem.begin();   /* Setup File IO */

}


void loop() {
  Bridge.get("color", colorbuffer, 256); 

//  if (color == ""){color = "rainbow";}
  if (String(colorbuffer).length() > 0 && color != String(colorbuffer)){
    color = String(colorbuffer);  

    Serial.println("Color: "); // so I can keep track of what is loaded
    Serial.println(color); // so I can keep track of what is loaded

    showColor(color);
  } else {
        if (color == "knightrider") {modeKnightRider();}
        if (color == "rainbow") {modeRainbow(1);}
        if (color == "fireplace") {modeFireplace();}
        if (color == "polizei") {modePolizei();}
        if (color == "text") {modeText();}
        if (color == "mail") {modeMail();}
        if (color == "pacman") {modePacman();}
        if (color == "invaders") {modeInvaders();}
        if (color == "hearts") {modeHearts();}
  }
}

void showColor(String color){
        if (color == "black") {colorWipe(strip.Color(0, 0, 0), 0);}   
        if (color == "blue") {colorWipe(strip.Color(0, 0, 255), 0);}   
        if (color == "cyan") {colorWipe(strip.Color(0, 255, 255), 0);}   
        if (color == "gray") {colorWipe(strip.Color(128, 128, 128), 0);}   
        if (color == "green") {colorWipe(strip.Color(0, 128, 0), 0);}   
        if (color == "lime") {colorWipe(strip.Color(0, 255, 0), 0);}   
        if (color == "magenta") {colorWipe(strip.Color(255, 0, 255), 0);}   
        if (color == "maroon") {colorWipe(strip.Color(128, 0, 0), 0);}   
        if (color == "navy") {colorWipe(strip.Color(0, 0, 128), 0);}   
        if (color == "olive") {colorWipe(strip.Color(128, 128, 0), 0);}   
        if (color == "oranje") {colorWipe(strip.Color(255, 165, 0), 0);}   
        if (color == "purple") {colorWipe(strip.Color(128, 0, 128), 0);}   
        if (color == "red") {colorWipe(strip.Color(255, 0, 0), 0);}   
        if (color == "silver") {colorWipe(strip.Color(192, 192, 192), 0);}   
        if (color == "teal") {colorWipe(strip.Color(0, 128, 128), 0);}   
        if (color == "white") {colorWipe(strip.Color(255, 255, 255), 0);}   
        if (color == "yellow") {colorWipe(strip.Color(255, 255, 0), 0);}   
}

void modePacman(){
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  uint16_t colorGreen   = matrix.Color(0, 255, 0);
  uint16_t colorBlue    = matrix.Color(0, 0, 255);
  uint16_t colorRed     = matrix.Color(255, 0, 0);

  drawPacman(x);
  drawPacmanGhost(x + 8, colorRed);
  drawPacmanGhost(x + 16, colorGreen);
  drawPacmanGhost(x + 24, colorBlue);

  if(--x < -36) {
    x = matrix.width();
    if(++pass >= 3) pass = 0;
  }

  matrix.show();
  delay(200);
}

void modeHearts(){
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);

  drawHeart(x);  
//  drawHeart(x + 9);  
//  drawHeart(x + 17);  

  if(--x < -36) {
    x = matrix.width();
    if(++pass >= 3) pass = 0;
  }

  matrix.show();
  delay(200);
}

void modeInvaders(){
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  uint16_t colorCyan     = matrix.Color(33, 255, 255);
  uint16_t colorLilac    = matrix.Color(251, 0, 255);
  uint16_t colorYellow   = matrix.Color(255, 255, 11);

  drawInvader1(x, colorCyan);
  drawInvader2(x + 10, colorLilac);
  drawInvader3(x + 17, colorYellow);

  if(--x < -32) {
    x = matrix.width();
    if(++pass >= 3) pass = 0;
  }

  matrix.show();
  delay(100);
}

void drawPacman(int xPos){
  if (xPos == 1 || xPos == 3 || xPos == 5 || xPos == 7) {
    uint16_t colorYellow  = matrix.Color(255, 255, 0);
    matrix.drawLine(xPos + 1,0, xPos + 3,0, colorYellow); // Line 1 
    matrix.drawLine(xPos + 0,1, xPos + 4,1, colorYellow); // Line 2 
    matrix.drawLine(xPos + 1,2, xPos + 5,2, colorYellow); // Line 3 
    matrix.drawLine(xPos + 2,3, xPos + 5,3, colorYellow); // Line 4 
    matrix.drawLine(xPos + 1,4, xPos + 5,4, colorYellow); // Line 5 
    matrix.drawLine(xPos + 0,5, xPos + 4,5, colorYellow); // Line 6 
    matrix.drawLine(xPos + 1,6, xPos + 4,6, colorYellow); // Line 7 
  } else {
    uint16_t colorYellow  = matrix.Color(255, 255, 0);
    matrix.drawLine(xPos + 1,0, xPos + 3,0, colorYellow); // Line 1 
    matrix.drawLine(xPos + 0,1, xPos + 4,1, colorYellow); // Line 2 
    matrix.drawLine(xPos + 0,2, xPos + 5,2, colorYellow); // Line 3 
    matrix.drawLine(xPos + 0,3, xPos + 5,3, colorYellow); // Line 4 
    matrix.drawLine(xPos + 0,4, xPos + 5,4, colorYellow); // Line 5 
    matrix.drawLine(xPos + 0,5, xPos + 4,5, colorYellow); // Line 6 
    matrix.drawLine(xPos + 1,6, xPos + 4,6, colorYellow); // Line 7 
  }
}

void drawPacmanGhost(int xPos, uint16_t color){
  matrix.drawLine(xPos + 1,0, xPos + 3,0, color); // Line 1 
  matrix.drawLine(xPos + 0,1, xPos + 4,1, color); // Line 2 
  matrix.drawLine(xPos + 0,2, xPos + 0,2, color); // Line 3 
  matrix.drawLine(xPos + 2,2, xPos + 2,2, color);
  matrix.drawLine(xPos + 4,2, xPos + 4,2, color);
  matrix.drawLine(xPos + 0,3, xPos + 4,3, color); // Line 4 
  matrix.drawLine(xPos + 0,4, xPos + 4,4, color); // Line 5 
  matrix.drawLine(xPos + 0,5, xPos + 4,5, color); // Line 6 
  matrix.drawLine(xPos + 0,6, xPos + 0,6, color); // Line 7 
  matrix.drawLine(xPos + 2,6, xPos + 2,6, color);
  matrix.drawLine(xPos + 4,6, xPos + 4,6, color);
}
  
void drawInvader1(int xPos, uint16_t color){
  matrix.drawLine(xPos + 2,0, xPos + 2,0, color); // Line 1 
  matrix.drawLine(xPos + 6,0, xPos + 6,0, color);
  matrix.drawLine(xPos + 3,1, xPos + 5,1, color); // Line 2 
  matrix.drawLine(xPos + 2,2, xPos + 2,2, color); // Line 3 
  matrix.drawLine(xPos + 4,2, xPos + 4,2, color);
  matrix.drawLine(xPos + 6,2, xPos + 6,2, color);
  matrix.drawLine(xPos + 1,3, xPos + 7,3, color); // Line 4 
  matrix.drawLine(xPos + 0,4, xPos + 0,4, color); // Line 5 
  matrix.drawLine(xPos + 2,4, xPos + 6,4, color);
  matrix.drawLine(xPos + 8,4, xPos + 8,4, color);
  matrix.drawLine(xPos + 0,5, xPos + 0,5, color); // Line 6 
  matrix.drawLine(xPos + 2,5, xPos + 2,5, color);
  matrix.drawLine(xPos + 6,5, xPos + 6,5, color);
  matrix.drawLine(xPos + 8,5, xPos + 8,5, color);
  matrix.drawLine(xPos + 2,6, xPos + 3,6, color); // Line 7 
  matrix.drawLine(xPos + 5,6, xPos + 6,6, color);
}

void drawInvader2(int xPos, uint16_t color){
  matrix.drawLine(xPos + 2,0, xPos + 2,0, color); // Line 1 
  matrix.drawLine(xPos + 1,1, xPos + 3,1, color); // Line 2 
  matrix.drawLine(xPos + 0,2, xPos + 0,2, color); // Line 3 
  matrix.drawLine(xPos + 2,2, xPos + 2,2, color);
  matrix.drawLine(xPos + 4,2, xPos + 4,2, color);
  matrix.drawLine(xPos + 0,3, xPos + 4,3, color); // Line 4 
  matrix.drawLine(xPos + 1,4, xPos + 1,4, color); // Line 5 
  matrix.drawLine(xPos + 3,4, xPos + 3,4, color);
  matrix.drawLine(xPos + 0,5, xPos + 0,6, color); // Line 6 
  matrix.drawLine(xPos + 2,5, xPos + 2,6, color);
  matrix.drawLine(xPos + 4,5, xPos + 4,6, color);
}

void drawInvader3(int xPos, uint16_t color){
  matrix.drawLine(xPos + 1,0, xPos + 3,0, color); // Line 1 
  matrix.drawLine(xPos + 0,1, xPos + 4,1, color); // Line 2 
  matrix.drawLine(xPos + 0,2, xPos + 0,2, color); // Line 3 
  matrix.drawLine(xPos + 2,2, xPos + 2,2, color);
  matrix.drawLine(xPos + 4,2, xPos + 4,2, color);
  matrix.drawLine(xPos + 0,3, xPos + 4,3, color); // Line 4 
  matrix.drawLine(xPos + 1,4, xPos + 1,5, color); // Line 5 + 6 
  matrix.drawLine(xPos + 3,4, xPos + 3,5, color); // Line 5 + 6
  matrix.drawLine(xPos + 0,6, xPos + 1,6, color); // Line 7 
  matrix.drawLine(xPos + 3,6, xPos + 4,6, color);
}

void drawHeart(int xPos){
  uint16_t colorMaroon   = matrix.Color(128, 0, 0);
  uint16_t colorRed      = matrix.Color(255, 0, 0);

  matrix.drawLine(xPos + 1,0, xPos + 3,0, colorMaroon); // Line 1 
  matrix.drawLine(xPos + 5,0, xPos + 7,0, colorMaroon);  

  matrix.drawLine(xPos + 0,1, xPos + 0,1, colorMaroon); // Line 2
  matrix.drawLine(xPos + 1,1, xPos + 2,1, colorRed); 
  matrix.drawLine(xPos + 3,1, xPos + 3,1, colorMaroon);
  matrix.drawLine(xPos + 5,1, xPos + 5,1, colorMaroon);
  matrix.drawLine(xPos + 6,1, xPos + 7,1, colorRed); 
  matrix.drawLine(xPos + 8,1, xPos + 8,1, colorMaroon);
  
  matrix.drawLine(xPos + 0,2, xPos + 0,2, colorMaroon); // Line 3 
  matrix.drawLine(xPos + 1,2, xPos + 3,2, colorRed); 
  matrix.drawLine(xPos + 4,2, xPos + 4,2, colorMaroon); // Line 3 
  matrix.drawLine(xPos + 5,2, xPos + 7,2, colorRed);
  matrix.drawLine(xPos + 8,2, xPos + 8,2, colorMaroon);

  matrix.drawLine(xPos + 0,3, xPos + 0,3, colorMaroon); // Line 4 
  matrix.drawLine(xPos + 1,3, xPos + 7,3, colorRed); 
  matrix.drawLine(xPos + 8,3, xPos + 8,3, colorMaroon); 

  matrix.drawLine(xPos + 1,4, xPos + 1,4, colorMaroon); // Line 5 
  matrix.drawLine(xPos + 2,4, xPos + 6,4, colorRed);
  matrix.drawLine(xPos + 7,4, xPos + 7,4, colorMaroon);
  
  matrix.drawLine(xPos + 2,5, xPos + 2,5, colorMaroon); // Line 6 
  matrix.drawLine(xPos + 3,5, xPos + 5,5, colorRed);
  matrix.drawLine(xPos + 6,5, xPos + 6,5, colorMaroon);

  matrix.drawLine(xPos + 3,6, xPos + 3,6, colorMaroon); // Line 7 
  matrix.drawLine(xPos + 4,6, xPos + 4,6, colorRed); 
  matrix.drawLine(xPos + 5,6, xPos + 5,6, colorMaroon); 

  matrix.drawLine(xPos + 4,7, xPos + 4,7, colorMaroon); // Line 8 
}

void modeMail(){
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  uint16_t color = matrix.Color(255, 0, 0);
  matrix.drawLine(x + 0,0, x + 0,5, color); // Left
  matrix.drawLine(x + 0,0, x + 6,0, color); // Top
  matrix.drawLine(x + 0,5, x + 6,5, color); // Bottom
  matrix.drawLine(x + 6,0, x + 6,5, color); // Right
  matrix.drawLine(x + 1,1, x + 3,3, color); // Fold 1
  matrix.drawLine(x + 3,3, x + 5,1, color); // Fold 2

  if(--x < -16) {
    x = matrix.width();
    if(++pass >= 3) pass = 0;
    matrix.setTextColor(matrixColors[pass]);
  }

  matrix.show();
  delay(100);

}

void modeText() {
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F("TECHINC"));
  if(--x < -36) {
    x = matrix.width();
    if(++pass >= 3) pass = 0;
    matrix.setTextColor(matrixColors[pass]);
  }
  matrix.show();
  delay(100);
}

void modePolizei(){
	for(uint8_t i = 0; i< 8; i++){
          colorWipe(strip.Color(0, 0, 0), 0);

        if (i < 4){
          strip.setPixelColor(0 + i, 255, 0, 0);
          strip.setPixelColor(8 + i, 255, 0, 0);
          strip.setPixelColor(16 + i, 255, 0, 0);
          strip.setPixelColor(24 + i, 255, 0, 0);
          strip.setPixelColor(32 + i, 255, 0, 0);
          strip.setPixelColor(40 + i, 255, 0, 0);
          strip.setPixelColor(48 + i, 255, 0, 0);
        } else {
          strip.setPixelColor(0 + i, 0, 0, 255);
          strip.setPixelColor(8 + i, 0, 0, 255);
          strip.setPixelColor(16 + i, 0, 0, 255);
          strip.setPixelColor(24 + i, 0, 0, 255);
          strip.setPixelColor(32 + i, 0, 0, 255);
          strip.setPixelColor(40 + i, 0, 0, 255);
          strip.setPixelColor(48 + i, 0, 0, 255);
        }
  
          if (i == 8)  {
  	  i = 0;
  	}
        strip.show();
    }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


void modeKnightRider() {
    colorWipe(strip.Color(0, 0, 0), 0);
    int loops = 1;
    int count = 0;
    int timer = 2;
  
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


void modeFireplace(){

	uint8_t iRandom = 0;
	for(int iRow = 0; iRow<8; iRow++){

		for(int iCol = 0; iCol<9; iCol++){
                for(int iBlank = 0; iBlank < strip.numPixels(); iBlank++){strip.setPixelColor(iRow+(iCol*8), 0,0,0);}            
			iRandom = random(1,100);
			if (iRandom > 25){strip.setPixelColor(iCol, 220, 0, 0);}
			if (iRandom > 50){strip.setPixelColor(8+iCol, 255, 35, 0);}
			if (iRandom > 65){strip.setPixelColor(16+iCol, 255, 75, 0);}
			if (iRandom > 80){strip.setPixelColor(24+iCol, 255, 100, 0);}
			if (iRandom > 90){strip.setPixelColor(32+iCol, 255, 150, 0);}
			if (iRandom > 95){strip.setPixelColor(40+iCol, 255, 200, 0);}
			if (iRandom > 98){strip.setPixelColor(48+iCol, 255, 255, 255);}
//                  delay(1);
		}
	}
        strip.show();
}

// Slightly different, this makes the rainbow equally distributed throughout
void modeRainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
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


