#include "Colorduino.h"

#define  TONE_C     261
#define  TONE_CIS   277
#define  TONE_DES   277
#define  TONE_D     294
#define  TONE_DIS   311
#define  TONE_ES    311
#define  TONE_E     330
#define  TONE_F     349
#define  TONE_FIS   370
#define  TONE_GES   370
#define  TONE_G     392
#define  TONE_GIS   415
#define  TONE_AS    415
#define  TONE_A     440
#define  TONE_AIS   466
#define  TONE_B     466
#define  TONE_H     494

#define  PAUSE      0

#define  TONE_LENGTH 50000

const int startButtonIn = 2;  // D2
const int upButtonIn = 19;    // A5
const int downButtonIn = 18;  // A4

const int speakerOut = 17;    // A3
const int cameraShotOut = 5;  // D5

unsigned int wait = 100;
unsigned int count_down_start = 10;
unsigned int count_down = count_down_start;
unsigned long time_before, time_after;

unsigned int pixel_r = 0;
unsigned int pixel_g = 0;
unsigned int pixel_b = 0;
unsigned int freq = 0;

extern unsigned char font8_3[10][3];

void slideNumber( int number, unsigned char r, unsigned char g, unsigned char b, int speed, int next = -1, int direction = 1 );

int melody[] = { TONE_C, TONE_G, TONE_D };
int beats[]  = { 1, 1, 2 };

void setup() {
  pinMode(startButtonIn, INPUT);
  pinMode(upButtonIn, INPUT);
  pinMode(downButtonIn, INPUT);
  
  pinMode(speakerOut, OUTPUT);
  pinMode(cameraShotOut, OUTPUT);
  
  Colorduino.Init();
}

void loop() {
  pixel_r = 0;
  pixel_g = 0;
  pixel_b = 100;

  freq = 0;

  count_down = count_down_start;

  showNumber( count_down, pixel_r, pixel_g, pixel_b );

  if (digitalRead(upButtonIn) == HIGH) {
    upButtonInPressed();
  } else if (digitalRead(downButtonIn) == HIGH) {
    downButtonInPressed();
  } else if (digitalRead(startButtonIn) == HIGH) {
    startButtonInPressed();
  }

  delay(10);
}

void upButtonInPressed() {
  if (count_down_start == 99) return;

  delay(10);

  if (digitalRead(upButtonIn) == HIGH) {
    playTone( TONE_D*2, TONE_LENGTH );

    slideNumber(count_down_start, pixel_r, pixel_g, pixel_b, 30, 1, -1);
    count_down_start += 1;

    while (digitalRead(upButtonIn) == HIGH) {
      delay(10);
    }
    delay(50);
  }
}

void downButtonInPressed() {
  if (count_down_start == 0) return;

  delay(10);

  if (digitalRead(downButtonIn) == HIGH) {
    playTone( TONE_D/2, TONE_LENGTH );

    slideNumber(count_down_start, pixel_r, pixel_g, pixel_b, 30);
    count_down_start -= 1;

    while (digitalRead(downButtonIn) == HIGH) {
      delay(10);
    }
    delay(50);
  }
}

void startButtonInPressed() {
  delay(10);

  if (digitalRead(startButtonIn) == HIGH) {
    playMelody( melody, beats );

    countDown();

    while (digitalRead(startButtonIn) == HIGH) {
      delay(10);
    }
    delay(50);
  }
}

/********************************************************
 * Name:
 *   countDown
 *
 * Function:
 *   counts Down form count_down_start to zero and call
 *   countDownEnded function
 *
 * Parameter:
 ********************************************************/
void countDown() {
  pixel_r = 0;
  pixel_g = 100;
  pixel_b = 0;

  freq = TONE_C;

  for (count_down; count_down > 0; count_down--) {

    time_before = millis();
    if (count_down <= 4) {
      pixel_r = 100;
      pixel_g = 0;
      pixel_b = 0;
      freq = TONE_C * 2;
    }
    slideNumber(count_down, pixel_r, pixel_g, pixel_b, 30);

    playTone( freq, TONE_LENGTH );

    time_after = millis();
    delay( 1000 - (time_after-time_before) );
  }

  countDownEnded();

  count_down = count_down_start;
}

/********************************************************
 * Name:
 *   countDownEnded
 *
 * Function:
 *   sets cameraShotOut to High, wenn count down ended
 *
 * Parameter:
 ********************************************************/
void countDownEnded() {
  digitalWrite(cameraShotOut, HIGH);

  blinkNumber(count_down, pixel_r, pixel_g, pixel_b, 100, 100, 5);

  digitalWrite(cameraShotOut, LOW);
}

/********************************************************
 * Name:
 *   playMelody
 *
 * Function:
 *   plays some tones
 *
 * Parameter:
 *   array of tones
 ********************************************************/
void playMelody( int* melody, int* beat ) {
  long duration = 0;
  int pause = 1000;

  for ( int i = 0; i < sizeof(melody) ; i++ ) {

    duration = beat[i] * TONE_LENGTH;

    playTone( melody[i], duration );

    delayMicroseconds(pause);
  }

  noTone( speakerOut );
}

/********************************************************
 * Name:
 *   playMelody
 *
 * Function:
 *   plays some tones
 *
 * Parameter:
 *   array of tones
 ********************************************************/
void playTone( int freq, long duration ) {

  if (freq > 0) {
    float tone = (1 / float(freq)) * 1000 * 1000;

    long elapsed_time = 0;

    while (elapsed_time < duration) {
      digitalWrite( speakerOut, HIGH );
      delayMicroseconds(tone / 2);
      digitalWrite( speakerOut, LOW );
      delayMicroseconds(tone / 2);
      elapsed_time += (tone);
    }

  } else {
    delayMicroseconds( duration );
  }
  
}

/********************************************************
 * Name:
 *   showNumber
 *
 * Function:
 *   display a number with two digits in LED matrix
 *
 * Parameter:
 *   number: the number want to show
 *   r: the value of r. Range: 0~255
 *   g: the value of g. Range: 0~255
 *   b: the value of b. Range: 0~255
 ********************************************************/
void showNumber( int number, unsigned char r, unsigned char g, unsigned char b ) {
  unsigned char chrtemp[8] = {0};

  int msb = number / 10 % 10;
  int lsb = number % 10;

  chrtemp[0] = pgm_read_byte(&(font8_3[msb][0]));
  chrtemp[1] = pgm_read_byte(&(font8_3[msb][1]));
  chrtemp[2] = pgm_read_byte(&(font8_3[msb][2]));
  chrtemp[3] = 0x00;
  chrtemp[4] = 0x00;
  chrtemp[5] = pgm_read_byte(&(font8_3[lsb][0]));
  chrtemp[6] = pgm_read_byte(&(font8_3[lsb][1]));
  chrtemp[7] = pgm_read_byte(&(font8_3[lsb][2]));

  Colorduino.ColorFillMask( chrtemp, r, g, b );

  Colorduino.FlipPage();
}

/********************************************************
 * Name:
 *   slideNumber
 *
 * Function:
 *   display a number with to digits in LED matrix and slide down to previous number
 *
 * Parameter:
 *   number: the number want to show
 *   r: the value of r. Range: 0~255
 *   g: the value of g. Range: 0~255
 *   b: the value of b. Range: 0~255
 *   speed: milliseconds between each step
 ********************************************************/
void slideNumber( int number, unsigned char r, unsigned char g, unsigned char b, int speed, int next, int direction ) {
  unsigned char i,j,Page_Write,temp;
  unsigned char chrtemp[8] = {0};

  int msb = number / 10 % 10;
  int lsb = number % 10;

  int msb_next = (number + next) / 10 % 10;
  int lsb_next = (number + next)  % 10;

  chrtemp[3] = 0x00;
  chrtemp[4] = 0x00;

  if ( direction > 0) {
    unsigned short col0 = pgm_read_byte(&(font8_3[msb_next][0])) << 8 | pgm_read_byte(&(font8_3[msb][0]));
    unsigned short col1 = pgm_read_byte(&(font8_3[msb_next][1])) << 8 | pgm_read_byte(&(font8_3[msb][1]));
    unsigned short col2 = pgm_read_byte(&(font8_3[msb_next][2])) << 8 | pgm_read_byte(&(font8_3[msb][2]));

    unsigned short col5 = pgm_read_byte(&(font8_3[lsb_next][0])) << 8 | pgm_read_byte(&(font8_3[lsb][0]));
    unsigned short col6 = pgm_read_byte(&(font8_3[lsb_next][1])) << 8 | pgm_read_byte(&(font8_3[lsb][1]));
    unsigned short col7 = pgm_read_byte(&(font8_3[lsb_next][2])) << 8 | pgm_read_byte(&(font8_3[lsb][2]));

    for (int bias = 0; bias <= 8; bias++) {
      if (msb == msb_next) {
        chrtemp[0] = col0;
        chrtemp[1] = col1;
        chrtemp[2] = col2;
      } else {
        chrtemp[0] = ( col0 ) >> bias;
        chrtemp[1] = ( col1 ) >> bias;
        chrtemp[2] = ( col2 ) >> bias;
      }

      chrtemp[5] = ( col5 ) >> bias;
      chrtemp[6] = ( col6 ) >> bias;
      chrtemp[7] = ( col7 ) >> bias;

      Colorduino.ColorFillMask( chrtemp, r, g, b );

      Colorduino.FlipPage();

      delay(speed);
    }
  } else if ( direction < 0) {
    unsigned short col0 = pgm_read_byte(&(font8_3[msb][0])) << 8 | pgm_read_byte(&(font8_3[msb_next][0]));
    unsigned short col1 = pgm_read_byte(&(font8_3[msb][1])) << 8 | pgm_read_byte(&(font8_3[msb_next][1]));
    unsigned short col2 = pgm_read_byte(&(font8_3[msb][2])) << 8 | pgm_read_byte(&(font8_3[msb_next][2]));

    unsigned short col5 = pgm_read_byte(&(font8_3[lsb][0])) << 8 | pgm_read_byte(&(font8_3[lsb_next][0]));
    unsigned short col6 = pgm_read_byte(&(font8_3[lsb][1])) << 8 | pgm_read_byte(&(font8_3[lsb_next][1]));
    unsigned short col7 = pgm_read_byte(&(font8_3[lsb][2])) << 8 | pgm_read_byte(&(font8_3[lsb_next][2]));

    for (int bias = 8; bias >= 0; bias--) {
      if (msb == msb_next) {
        chrtemp[0] = col0;
        chrtemp[1] = col1;
        chrtemp[2] = col2;
      } else {
        chrtemp[0] = ( col0 ) >> bias;
        chrtemp[1] = ( col1 ) >> bias;
        chrtemp[2] = ( col2 ) >> bias;
      }

      chrtemp[5] = ( col5 ) >> bias;
      chrtemp[6] = ( col6 ) >> bias;
      chrtemp[7] = ( col7 ) >> bias;

      Colorduino.ColorFillMask( chrtemp, r, g, b );

      Colorduino.FlipPage();

      delay(speed);
    }
  }
}

/********************************************************
 * Name:
 *   blinkNumber
 *
 * Function:
 *   lets blink a number with two digits in LED matrix
 *
 * Parameter:
 *   number: the number want to show
 *   r: the value of r. Range: 0~255
 *   g: the value of g. Range: 0~255
 *   b: the value of b. Range: 0~255
 *   delay_on: milliseconds of displaying the number
 *   delay_off: milliseconds of not displaying the number
 *   count: number how often the given number should blink
 ********************************************************/
void blinkNumber( int number, unsigned char r, unsigned char g, unsigned char b, int delay_on, int delay_off, int count ) {
  int msb = number / 10 % 10;
  int lsb = number % 10;

  for (count; count > 0; count--) {
    freq = TONE_C * 4;

    playTone( freq, TONE_LENGTH );

    showNumber( number, r, g, b );

    delay(delay_on);

    showNumber( 0, 0, 0, 0 );

    delay(delay_off);
  }
}

