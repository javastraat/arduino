#include <Colorduino.h>
#include <Wire.h>

#define  MATRIX_STATE_OFF     0x20
#define  MATRIX_STATE_FILL    0x21
#define  MATRIX_STATE_PLASMA  0x22
#define  MATRIX_STATE_FLASH   0x23
#define  MATRIX_STATE_RAINBOW 0x24

typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} ColorRGB;

typedef struct {
  unsigned char h; // need to be WORD / INT?
  unsigned char s;
  unsigned char v;
} ColorHSV;

typedef struct {
  char      state;
  ColorRGB  rgb;
  ColorHSV  hsv;
  char      speed;
  char      effect;
  char      number;
  boolean   done;
} commandData;

// Plasma
unsigned char plasma[8][8];
long paletteShift;

unsigned int  counter;
commandData   matrix, matrixTemp;
unsigned long time;


void setup() {
  Serial.begin(9600);
  
  Colorduino.Init(); // initialize the board
  unsigned char whiteBalVal[3] = {30,63,63}; // for LEDSEE 6x6cm round matrix
  Colorduino.SetWhiteBal(whiteBalVal);
  
  paletteShift=128000;
  unsigned char bcolor;

  //generate the plasma once
  for(unsigned char y = 0; y < 8; y++) {
    for(unsigned char x = 0; x < 8; x++) {
      bcolor = (unsigned char)
      (
            128.0 + (128.0 * sin(x*8.0 / 16.0))
          + 128.0 + (128.0 * sin(y*8.0 / 16.0))
      ) / 2;
      plasma[x][y] = bcolor;
    }
  }
}


void loop() {
  receiveSerial();
  
  switch (matrix.state) {
    case MATRIX_STATE_OFF:     matrixOff();     break;
    case MATRIX_STATE_FILL:    matrixFillTest();    break;
    case MATRIX_STATE_PLASMA:  matrixPlasma();  break;
    case MATRIX_STATE_FLASH:   matrixFlash();   break;
    case MATRIX_STATE_RAINBOW: matrixRainbow(); break;
  }
}

