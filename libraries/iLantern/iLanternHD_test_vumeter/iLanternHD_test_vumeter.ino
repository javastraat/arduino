#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz) 
unsigned int sample;

#define PIN 6
#define NUM_LIGHTS 300
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(25, 12, PIN,
  NEO_MATRIX_BOTTOM  + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS    + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

void setup()
{ 
  Serial.begin(9600); 
  matrix.begin();
  matrix.fillScreen(0);

//  matrix.setBrightness(40);
//  matrix.setTextSize(1);
}

void loop()
{
  unsigned long startMillis= millis(); // Start of sample window 
  unsigned int peakToPeak = 0; // peak-to-peak level
  unsigned int signalMax = 0; 
  unsigned int signalMin = 1024;
  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(0);
    if (sample < 1024) // toss out spurious readings 
    {
      if (sample > signalMax)
      {
        signalMax = sample; // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample; // save just the min levels
      } 
     }
  }
  peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude 
  double volts = (peakToPeak * 3.3) / 100; // convert to volts
  Serial.print("\n");
  Serial.print(volts);
  modeVUMeter(volts);
}

void drawVUBar(int yPos, uint32_t color){
  matrix.drawLine(0,yPos,25,yPos, color);
}

void modeVUMeter(double volts){
    
//  matrix.fillScreen(0);
  matrix.setCursor(0, 0);

    if (volts > 0.3){drawVUBar(11, matrix.Color(21, 255, 0));} else {drawVUBar(11, matrix.Color(0, 0, 0));}
    if (volts > 0.4){drawVUBar(10, matrix.Color(42, 212, 0));} else {drawVUBar(10, matrix.Color(0, 0, 0));}
    if (volts > 0.5){drawVUBar(9, matrix.Color(53, 191, 0));} else {drawVUBar(9, matrix.Color(0, 0, 0));}

    if (volts > 0.6){drawVUBar(8, matrix.Color(64, 170, 0));} else {drawVUBar(8, matrix.Color(0, 0, 0));}
    if (volts > 0.7){drawVUBar(7, matrix.Color(85, 148, 0));} else {drawVUBar(7, matrix.Color(0, 0, 0));}
    if (volts > 0.8){drawVUBar(6, matrix.Color(106, 127, 0));} else {drawVUBar(6, matrix.Color(0, 0, 0));}

    if (volts > 0.9){drawVUBar(5, matrix.Color(127, 106, 0));} else {drawVUBar(5, matrix.Color(0, 0, 0));}
    if (volts > 1.0){drawVUBar(4, matrix.Color(148, 85, 0));} else {drawVUBar(4, matrix.Color(0, 0, 0));}
    if (volts > 1.1){drawVUBar(3, matrix.Color(170, 64, 0));} else {drawVUBar(3, matrix.Color(0, 0, 0));}

    if (volts > 1.2){drawVUBar(2, matrix.Color(191, 53, 0));} else {drawVUBar(2, matrix.Color(0, 0, 0));}
    if (volts > 1.3){drawVUBar(1, matrix.Color(212, 42, 0));} else {drawVUBar(1, matrix.Color(0, 0, 0));}
    if (volts > 1.4){drawVUBar(0, matrix.Color(255, 21, 0));} else {drawVUBar(0, matrix.Color(0, 0, 0));}

  matrix.show();

}


void colorWipe(uint32_t c, uint8_t wait) { 
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

