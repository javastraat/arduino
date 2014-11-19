////////////////////////////////////////////////
// Character Display
// Displays characters one at a time on the 8x8 Matrix
// This code may be freely used and copied.
//
// Gareth Davies - June 2012
//
////////////////////////////////////////////////

// pin[xx] on led matrix connected to nn on Arduino (-1 is dummy to make array start at pos 1)
int pins[17] = { -1, 5, 4, 3, 2,  14, 15, 16, 17,  13, 12, 11, 10,   9, 8, 7, 6};

// col[xx] of leds = pin yy on led matrix
int colRed[8] = {pins[13], pins[3], pins[4], pins[10], pins[06], pins[11], pins[15], pins[16]};
//9,3,2,12,15,11,7,6

// row[xx] of leds = pin yy on led matrix
int comRows[8] = {pins[9], pins[14], pins[8], pins[12], pins[1], pins[7], pins[2], pins[5]};
//13,8,17,10,5,16,4,14


int idxCol = 0, idxRow = 0;
int delay1 = 2; // delay for each column in ms
int count = 0; // used for cycling through characters
long last = 0;
long now;

long interval = 50; // delay between each character in ms
const int waterChars = 11; //number of characters in array and to display

// The following 2-dimensional array defines the characters in binary format. '1' e LED on, '0' = LED off.
// Each line represents one row of LEDs, each group of 8 lines inside {braces} is a character
// Each character is separated by a comma

void waterdrip() {
  byte drip1[8] = {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111111,
    B11111111
  };
    
void waterdrip() {
  byte drip2[8] = {
    B00010000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111111,
    B11111111
  };
 
void waterdrip() {
  byte drip3[8] = {
    B00000000,
    B00010000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111111,
    B11111111
  };
  
void waterdrip() {
  byte drip4[8] = {
    B00000000,
    B00000000,
    B00010000,
    B00000000,
    B00000000,
    B00000000,
    B11111111,
    B11111111
  };
  
void waterdrip() {
  byte drip5[8] = {
    B00000000,
    B00000000,
    B00000000,
    B00010000,
    B00000000,
    B00000000,
    B11111111,
    B11111111
  };
  
void waterdrip() {
  byte drip6[8] = {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00010000,
    B00000000,
    B11111111,
    B11111111
  };
void waterdrip() {
  byte drip7[8] = {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00010000,
    B11111111,
    B11111111
  };
  
void waterdrip() {
  byte drip8[8] = {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11101111,
    B11111111
  };
  
void waterdrip() {
  byte drip9[8] = {
    B00000000, 
    B00000000, 
    B00000000, 
    B00000000, 
    B00000000, 
    B00101000, 
    B11010111, 
    B11111111
  };
  
void waterdrip() {
  byte drip10[8] = {
    B00000000, 
    B00000000, 
    B00000000, 
    B00000000, 
    B00000000, 
    B00010000, 
    B10101011, 
    B11111111
  };
  
void waterdrip() {
  byte drip11[8] = {
    B00000000, 
    B00000000, 
    B00000000, 
    B00000000, 
    B00000000, 
    B00000000, 
    B01111110, 
    B11111111
  }
};



const int blokChars = 10; //number of characters in array and to display


byte squareanim[blokChars][8] = {{
    B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111
  }, {
    B00000000, B01111110, B01111110, B01111110, B01111110, B01111110, B01111110, B00000000
  }, {
    B00000000, B00000000, B00111100, B00111100, B00111100, B00111100, B00000000, B00000000
  }, {
    B00000000, B00000000, B00000000, B00011000, B00011000, B00000000, B00000000, B00000000
  }, {
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
  }, {
    B00000000, B00000000, B00000000, B00011000, B00011000, B00000000, B00000000, B00000000
  }, {
    B00000000, B00000000, B00111100, B00111100, B00111100, B00111100, B00000000, B00000000
  }, {
    B00000000, B01111110, B01111110, B01111110, B01111110, B01111110, B01111110, B00000000
  }, {
    B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111
  }, {
    B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111
  }
};


const int alienChars = 20; //number of characters in array and to display


byte anim2text[alienChars][8] = {{

    // Animation bitmaps. Each frame of animation MUST contain
    // 8 lines of graphics data (there is no error checking for
    // length). Each line should be prefixed with the letter 'B',
    // followed by exactly 8 binary digits (0 or 1), no more,
    // no less (again, no error checking). '0' represents an
    // 'off' pixel, '1' an 'on' pixel. End line with a comma.
    B00011000, // This is the first frame for alien #1
    B00111100, // If you squint you can kind of see the
    B01111110, // image in the 0's and 1's.
    B11011011,
    B11111111,
    B00100100,
    B01011010,
    B10100101
    // The 9th line (required) is the time to display this frame,
    // in 1/100ths of a second (e.g. 100 = 1 sec, 25 = 1/4 sec,
    // etc.). Range is 0 (no delay) to 255 (2.55 seconds). If
    // longer delays are needed, make duplicate frames.
    //25, // 0.25 seconds
  }, {
    B00011000, // This is the second frame for alien #1
    B00111100,
    B01111110,
    B11011011,
    B11111111,
    B00100100,
    B01011010,
    B01000010
    //25, // 0.25 second delay
  }, {
    // Frames 3 & 4 for alien #1 are duplicates of frames 1 & 2.
    // Rather than list them 'the tall way' again, the lines are merged here...
    B00011000, B00111100, B01111110, B11011011, B11111111, B00100100, B01011010, B10100101
  }, {
    B00011000, B00111100, B01111110, B11011011, B11111111, B00100100, B01011010, B01000010
  }, {

    B00000000, // First frame for alien #2
    B00111100,
    B01111110,
    B11011011,
    B11011011,
    B01111110,
    B00100100,
    B11000011
    //25, // 0.25 second delay
  }, {
    B00111100, // Second frame for alien #2
    B01111110,
    B11011011,
    B11011011,
    B01111110,
    B00100100,
    B00100100,
    B00100100
    //25,
  }, {
    // Frames 3 & 4 for alien #2 are duplicates of frames 1 & 2
    B00000000, B00111100, B01111110, B11011011, B11011011, B01111110, B00100100, B11000011
  }, {
    B00111100, B01111110, B11011011, B11011011, B01111110, B00100100, B00100100, B00100100
  }, {

    B00100100, // First frame for alien #3
    B00100100,
    B01111110,
    B11011011,
    B11111111,
    B11111111,
    B10100101,
    B00100100
    //25,
  }, {
    B00100100, // Second frame for alien #3
    B10100101,
    B11111111,
    B11011011,
    B11111111,
    B01111110,
    B00100100,
    B01000010
    //25,
  }, {
    // Frames are duplicated as with prior aliens
    B00100100, B00100100, B01111110, B11011011, B11111111, B11111111, B10100101, B00100100
  }, {
    B00100100, B10100101, B11111111, B11011011, B11111111, B01111110, B00100100, B01000010
  }, {
    B00111100, // First frame for alien #4
    B01111110,
    B10110011,
    B01111110,
    B00111100,
    B00000000,
    B00001000,
    B00000000
    //12, // ~1/8 second delay
  }, {
    B00111100, // Second frame for alien #4
    B01111110,
    B10011001,
    B01111110,
    B00111100,
    B00000000,
    B00001000,
    B00001000
    //12,
  }, {
    B00111100, // Third frame for alien #4 (NOT a repeat of frame 1)
    B01111110,
    B11001101,
    B01111110,
    B00111100,
    B00000000,
    B00000000,
    B00001000
    //12,
  }, {
    B00111100, // Fourth frame for alien #4 (NOT a repeat of frame 2)
    B01111110,
    B11100111,
    B01111110,
    B00111100,
    B00000000,
    B00000000,
    B00000000
    //12,
  }, {
    // Frames 5-8 are duplicates of 1-4, lines merged for brevity
    B00111100, B01111110, B10110011, B01111110, B00111100, B00000000, B00001000, B00000000
  }, {
    B00111100, B01111110, B10011001, B01111110, B00111100, B00000000, B00001000, B00001000
  }, {
    B00111100, B01111110, B11001101, B01111110, B00111100, B00000000, B00000000, B00001000
  }, {
    B00111100, B01111110, B11100111, B01111110, B00111100, B00000000, B00000000, B00000000
  }
};


void setup() {
  // initialize all pins as outputs and disable everything
  for (int i = 0; i < 8; i++)
  {
    pinMode(comRows[i], OUTPUT);
    pinMode(colRed[i], OUTPUT);
    digitalWrite(comRows[i], LOW);   // switch ROW off (Anode)
    digitalWrite(colRed[i], HIGH);    // switch the column off (Cathode)
  }
  last = millis();
}

void loop()
{
  water(1000); //water dripping
  //anim1(1000); //square animation
  //anim2(1000);//alien animation
}




void water(uint8_t wait) {
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (bitRead(dripanim[count][i], j) == 1)
        digitalWrite(colRed[j], LOW);
      else
        digitalWrite(colRed[j], HIGH);
    }
    digitalWrite(comRows[i], HIGH); // enable row
    delay(delay1);
    digitalWrite(comRows[i], LOW); // disable row
  }
  now = millis();
  if ((now - last) > interval)
  {
    last = now;
    count++;
    if (count > (waterChars - 1))
      count = 0;
  }


}

void anim1(uint8_t wait) {
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (bitRead(squareanim[count][i], j) == 1)
        digitalWrite(colRed[j], LOW);
      else
        digitalWrite(colRed[j], HIGH);
    }
    digitalWrite(comRows[i], HIGH); // enable row
    delay(delay1);
    digitalWrite(comRows[i], LOW); // disable row
  }
  now = millis();
  if ((now - last) > interval)
  {
    last = now;
    count++;
    if (count > (blokChars - 1))
      count = 0;
  }

}

void anim2(uint8_t wait)
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (bitRead(anim2text[count][i], j) == 1)
        digitalWrite(colRed[j], LOW);
      else
        digitalWrite(colRed[j], HIGH);
    }
    digitalWrite(comRows[i], HIGH); // enable row
    delay(delay1);
    digitalWrite(comRows[i], LOW); // disable row
  }
  now = millis();
  if ((now - last) > interval)
  {
    last = now;
    count++;
    if (count > (alienChars - 1))
      count = 0;
  }
}


