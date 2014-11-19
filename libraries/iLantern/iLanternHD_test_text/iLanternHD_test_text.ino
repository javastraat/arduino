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
  matrix.setCursor(x, -1);
  String textstring = "Tech Inc";
  matrix.print(textstring);
  matrix.show();

  if(x < -100) {
    x = matrix.width() + 1;
  } else { 
    x--;
  }
  delay(100);
}


