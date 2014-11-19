#include <Sprite.h>  // Sprite before Matrix
#include <Matrix.h>

const int numChips = 2;

//                   DIN, CLK, LOAD, #chips
Matrix myLeds = Matrix(10, 12, 11, numChips);

void setup() {
  myLeds.clear();
}

void loop() {
  byte x, y;

  // light one LED at a time, scanning left to right
  // and top to bottom... useful for testing the matrix
  for (y=0; y<8; y++) {
    for (x=0; x<(numChips * 8); x++) {
      myLeds.write(x, y, HIGH);
      delay(50);
      myLeds.write(x, y, LOW);
    }
  }
}
