#include <Sprite.h>
#include <Matrix.h>

// Sprite Animation
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates the use of the Matrix & Sprite libraries
// Displays animated waveform graphic on screen

// Created 29 March 2006

/* create a new Matrix instance
   pin 0: data  (din)
   pin 1: load  (load)
   pin 2: clock (clk)
*/
Matrix myMatrix = Matrix(10, 12, 11);

/* create a new Sprite instance
   8 pixels wide, 4 pixels tall
*/
Sprite wave = Sprite(
  8, 8,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000
  
);

void setup()
{ 
}

int x = 0;

void loop()
{
  myMatrix.write(x, 0, wave);     // place sprite on screen
  myMatrix.write(x - 8, 0, wave); // place sprite again, elsewhere on screen
  delay(100);                      // wait a little bit
  myMatrix.clear();               // clear the screen for next animation frame
  if(x == 8)                      // if reached end of animation sequence
  {
    x = 0;                        // start from beginning
  }
  x++;                            // advance x coordinate to the right
}

