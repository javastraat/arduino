  #include <SPI.h>
#include <Bridge.h> 

#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>

#define PIN 6
#define NUM_LIGHTS 300
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(25, 12, PIN,
  NEO_MATRIX_BOTTOM  + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS    + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

int x    = matrix.width() + 1;

void setup() {


  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(matrix.Color(0, 0, 255)); 
  matrix.setTextSize(2);
  Serial.begin(9600); 
  Bridge.begin();

  strip.begin();
  strip.show();
}


void loop() {

  uint16_t colorWhite  = matrix.Color(255, 255, 255);
  uint16_t colorBlue   = matrix.Color(0, 0, 255);
  
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  String textstring = "ACTA ";
  matrix.print(textstring);
//  drawSkull(x, colorWhite);
  matrix.show();

  if(x < -36) {
    x = matrix.width() + 1;
  } else { 
    x--;
  }
  delay(100);
}

void drawSkull(int xPos, uint16_t color){
// Line 0

// Line 1
  matrix.drawLine(xPos + 3,1, xPos + 7,1, color);

// Line 2
  matrix.drawLine(xPos + 2,2, xPos + 8,2, color);

// Line 3
  matrix.drawLine(xPos + 1,3, xPos + 9,3, color);

// Line 4
  matrix.drawLine(xPos + 1,4, xPos + 1,4, color);
  matrix.drawLine(xPos + 4,4, xPos + 6,4, color);
  matrix.drawLine(xPos + 9,4, xPos + 9,4, color);

// Line 5
  matrix.drawLine(xPos + 1,5, xPos + 1,5, color);
  matrix.drawLine(xPos + 5,5, xPos + 5,5, color);
  matrix.drawLine(xPos + 9,5, xPos + 9,5, color);

// Line 6
  matrix.drawLine(xPos + 1,6, xPos + 9,6, color);

// Line 7
  matrix.drawLine(xPos + 2,7, xPos + 4,7, color);
  matrix.drawLine(xPos + 6,7, xPos + 8,7, color);

// Line 8
  matrix.drawLine(xPos + 3,8, xPos + 7,8, color);

// Line 9
  matrix.drawLine(xPos + 3,9, xPos + 3,9, color);
  matrix.drawLine(xPos + 5,9, xPos + 5,9, color);
  matrix.drawLine(xPos + 7,9, xPos + 7,9, color);

// Line 10
// Line 11

}


