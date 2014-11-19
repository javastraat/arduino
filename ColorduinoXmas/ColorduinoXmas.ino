/*

This sketch reads a string in from the serial port and 
displays that string on the LED matrix.

*/

#include <Adafruit_GFX.h>
#include <Colorduino_GFX.h>

int        counter = 0;
int        height = 8;
int        width = 8;
int        depth = 3;
GFX_Color_t  frameBuffer[8][8];

ColorduinoPanel Colorduino;

void setup() {
  Serial.begin(9600);
  
  Colorduino.init();
  fillFrame(0, 0, 0);
  
}

void loop() {
  // Vertical lines.
  for ( int w = 0; w < width; w=w+3 ) {
    Serial.print("Width: ");
    Serial.println( w );
    for ( int h = 0 ; h < height; h++ ) {
      Serial.print("height: ");
      Serial.println( h );
      frameBuffer[w][h] = Colorduino.color(counter,0,0); 
    }
  }

  // Horizontal lines.
  for ( int w = 0; w < width; w++ ) {
    Serial.print("Width: ");
    Serial.println( w );
    for ( int h = 0 ; h < height; h=h+2 ) {
      Serial.print("height: ");
      Serial.println( h );
      frameBuffer[w][h] = Colorduino.color(0,counter,0); 
    }
  }
  
  Serial.println("Drawframe");
  drawFrame();
  counter = counter + 10;
  delay(500);
}

/* Fill the frame with completely random data - for testing */
void fillFrame( int r, int g, int b ) {
  for ( int w = 0; w < width; w++ ) {
    for ( int h = 0 ; h < height; h++ ) {
       frameBuffer[w][h] = Colorduino.color(r, g, b); 
    }
  }
}

/* Fill the frame with completely random data - for testing */
void randomFrame() {
  for ( int w = 0; w < width; w++ ) {
    for ( int h = 0 ; h < height; h++ ) {
       frameBuffer[w][h] = Colorduino.color(random(256), random(256), random(256)); 
    }
  }
}

/* Draw the frame a pixel at a time */
void drawFrame() {
  for ( int w = 0; w < width; w++ ) {
    for ( int h = 0 ; h < height; h++ ) {
      // Yes, this is rotated.
      Colorduino.drawPixel(h, w, frameBuffer[w][h]);
    }
  }
  Colorduino.swapBuffers(false);
}

