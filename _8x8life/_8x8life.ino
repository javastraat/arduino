/*
 * Conway's "Life"
 *
 * Adapted from the Life example
 * on the Processing.org site
 *
 * Needs FrequencyTimer2 library
 */

#include <FrequencyTimer2.h>

byte col = 0;
byte leds[8][8];

// pin[xx] on led matrix connected to nn on Arduino (-1 is dummy to make array start at pos 1)
int pins[17]= {-1, 5, 4, 3, 2, 14, 15, 16, 17, 13, 12, 11, 10, 9, 8, 7, 6};
                    
// col[xx] of leds = pin yy on led matrix
int cols[8] = {pins[13], pins[3], pins[4], pins[10], pins[06], pins[11], pins[15], pins[16]};
//9,3,2,12,15,11,7,6

// row[xx] of leds = pin yy on led matrix
int rows[8] = {pins[9], pins[14], pins[8], pins[12], pins[1], pins[7], pins[2], pins[5]};
//13,8,17,10,5,16,4,14

#define DELAY 1000
#define SIZE 8
extern byte leds[SIZE][SIZE];
byte world[SIZE][SIZE][2];
long density = 50;

void setup() {
  setupLeds();
  randomSeed(analogRead(5));
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (random(100) < density) {
        world[i][j][0] = 1;
      }
      else {
        world[i][j][0] = 0;
      }
      world[i][j][1] = 0;
    }
  }
}

void loop() {
  // Display current generation
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      leds[i][j] = world[i][j][0];
    }
  }
  delay(DELAY);

  // Birth and death cycle
  for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
      // Default is for cell to stay the same
      world[x][y][1] = world[x][y][0];
      int count = neighbours(x, y);
      if (count == 3 && world[x][y][0] == 0) {
        // A new cell is born
        world[x][y][1] = 1;
      }
      if ((count < 2 || count > 3) && world[x][y][0] == 1) {
        // Cell dies
        world[x][y][1] = 0;
      }
    }
  }

  // Copy next generation into place
  for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
      world[x][y][0] = world[x][y][1];
    }
  }
}

int neighbours(int x, int y) {
 return world[(x + 1) % SIZE][y][0] +
         world[x][(y + 1) % SIZE][0] +
         world[(x + SIZE - 1) % SIZE][y][0] +
         world[x][(y + SIZE - 1) % SIZE][0] +
         world[(x + 1) % SIZE][(y + 1) % SIZE][0] +
         world[(x + SIZE - 1) % SIZE][(y + 1) % SIZE][0] +
         world[(x + SIZE - 1) % SIZE][(y + SIZE - 1) % SIZE][0] +
         world[(x + 1) % SIZE][(y + SIZE - 1) % SIZE][0];
}

void setupLeds() {
  // sets the pins as output
  for (int i = 1; i <= 16; i++) {
    pinMode(pins[i], OUTPUT);
  }

  // set up cols and rows
  for (int i = 1; i <= 8; i++) {
    digitalWrite(cols[i - 1], HIGH);
  }

  for (int i = 1; i <= 8; i++) {
    digitalWrite(rows[i - 1], HIGH);
  }

  clearLeds();

  // Turn off toggling of pin 11 and 3
  FrequencyTimer2::disable();
  // Set refresh rate (interrupt timeout period)
  FrequencyTimer2::setPeriod(2000);
  // Set interrupt routine to be called
  FrequencyTimer2::setOnOverflow(display);

}

void clearLeds() {
  // Clear display array
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      leds[i][j] = 0;
    }
  }
}

// Interrupt routine
void display() {
  digitalWrite(cols[col], HIGH);  // Turn whole previous column off
  col++;
  if (col == 8) {
    col = 0;
  }
  for (int row = 0; row < 8; row++) {
    if (leds[col][7 - row] == 1) {
      digitalWrite(rows[row], HIGH);  // Turn on this led
    }
    else {
      digitalWrite(rows[row], LOW); // Turn off this led
    }
  }
  digitalWrite(cols[col], LOW); // Turn whole column on at once (for equal lighting times)
}
