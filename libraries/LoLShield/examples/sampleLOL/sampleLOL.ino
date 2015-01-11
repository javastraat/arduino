#include <avr/pgmspace.h>  //This is in the Arduino library 

int blinkdelay = 75; //This basically controls brightness. Lower is dimmer
int runspeed = 20;   //smaller = faster

int pin13 =13;
int pin12 =12;
int pin11 =11;
int pin10 =10;
int pin09 =9;
int pin08 =8;
int pin07 =7;
int pin06 =6;
int pin05 =5;
int pin04 =4;
int pin03 =3;
int pin02 =2;

const int pins[] = {
  pin13,pin12,pin11,pin10,pin09,pin08,pin07,pin06,pin05,pin04,pin03,pin02};

const int ledMap[126][2] ={
{pin13, pin05},{pin13, pin06},{pin13, pin07},{pin13, pin08},{pin13, pin09},{pin13, pin10},{pin13, pin11},{pin13, pin12},{pin13, pin04},{pin04, pin13},{pin13, pin03},{pin03, pin13},{pin13, pin02},{pin02, pin13},
{pin12, pin05},{pin12, pin06},{pin12, pin07},{pin12, pin08},{pin12, pin09},{pin12, pin10},{pin12, pin11},{pin12, pin13},{pin12, pin04},{pin04, pin12},{pin12, pin03},{pin03, pin12},{pin12, pin02},{pin02, pin12},
{pin11, pin05},{pin11, pin06},{pin11, pin07},{pin11, pin08},{pin11, pin09},{pin11, pin10},{pin11, pin12},{pin11, pin13},{pin11, pin04},{pin04, pin11},{pin11, pin03},{pin03, pin11},{pin11, pin02},{pin02, pin11},
{pin10, pin05},{pin10, pin06},{pin10, pin07},{pin10, pin08},{pin10, pin09},{pin10, pin11},{pin10, pin12},{pin10, pin13},{pin10, pin04},{pin04, pin10},{pin10, pin03},{pin03, pin10},{pin10, pin02},{pin02, pin10},
{pin09, pin05},{pin09, pin06},{pin09, pin07},{pin09, pin08},{pin09, pin10},{pin09, pin11},{pin09, pin12},{pin09, pin13},{pin09, pin04},{pin04, pin09},{pin09, pin03},{pin03, pin09},{pin09, pin02},{pin02, pin09},
{pin08, pin05},{pin08, pin06},{pin08, pin07},{pin08, pin09},{pin08, pin10},{pin08, pin11},{pin08, pin12},{pin08, pin13},{pin08, pin04},{pin04, pin08},{pin08, pin03},{pin03, pin08},{pin08, pin02},{pin02, pin08},
{pin07, pin05},{pin07, pin06},{pin07, pin08},{pin07, pin09},{pin07, pin10},{pin07, pin11},{pin07, pin12},{pin07, pin13},{pin07, pin04},{pin04, pin07},{pin07, pin03},{pin03, pin07},{pin07, pin02},{pin02, pin07},
{pin06, pin05},{pin06, pin07},{pin06, pin08},{pin06, pin09},{pin06, pin10},{pin06, pin11},{pin06, pin12},{pin06, pin13},{pin06, pin04},{pin04, pin06},{pin06, pin03},{pin03, pin06},{pin06, pin02},{pin02, pin06},
{pin05, pin06},{pin05, pin07},{pin05, pin08},{pin05, pin09},{pin05, pin10},{pin05, pin11},{pin05, pin12},{pin05, pin13},{pin05, pin04},{pin04, pin05},{pin05, pin03},{pin03, pin05},{pin05, pin02},{pin02, pin05}
};

uint16_t BitMap[][9] PROGMEM = {
{1, 0, 0, 0, 0, 0, 0, 0, 0},
{3, 1, 0, 0, 0, 0, 0, 0, 0},
{7, 3, 1, 0, 0, 0, 0, 0, 0},
{15, 7, 3, 1, 0, 0, 0, 0, 0},
{31, 15, 7, 3, 1, 0, 0, 0, 0},
{63, 31, 15, 7, 3, 1, 0, 0, 0},
{127, 63, 31, 15, 7, 3, 1, 0, 0},
{255, 127, 63, 31, 15, 7, 3, 1, 0},
{511, 255, 127, 63, 31, 15, 7, 3, 1},
{1023, 511, 255, 127, 63, 31, 15, 7, 3},
{2047, 1023, 511, 255, 127, 63, 31, 15, 7},
{4095, 2047, 1023, 511, 255, 127, 63, 31, 15},
{8191, 4095, 2047, 1023, 511, 255, 127, 63, 31},
{16383, 8191, 4095, 2047, 1023, 511, 255, 127, 63},
{16383, 16383, 8191, 4095, 2047, 1023, 511, 255, 127},
{16383, 16383, 16383, 8191, 4095, 2047, 1023, 511, 255},
{16383, 16383, 16383, 16383, 8191, 4095, 2047, 1023, 511},
{16383, 16383, 16383, 16383, 16383, 8191, 4095, 2047, 1023},
{16383, 16383, 16383, 16383, 16383, 16383, 8191, 4095, 2047},
{16383, 16383, 16383, 16383, 16383, 16383, 16383, 8191, 4095},
{16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383, 8191},
{16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383},
{16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383},
{16382, 16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383},
{16380, 16382, 16383, 16383, 16383, 16383, 16383, 16383, 16383},
{16376, 16380, 16382, 16383, 16383, 16383, 16383, 16383, 16383},
{16368, 16376, 16380, 16382, 16383, 16383, 16383, 16383, 16383},
{16352, 16368, 16376, 16380, 16382, 16383, 16383, 16383, 16383},
{16320, 16352, 16368, 16376, 16380, 16382, 16383, 16383, 16383},
{16256, 16320, 16352, 16368, 16376, 16380, 16382, 16383, 16383},
{16128, 16256, 16320, 16352, 16368, 16376, 16380, 16382, 16383},
{15872, 16128, 16256, 16320, 16352, 16368, 16376, 16380, 16382},
{15360, 15872, 16128, 16256, 16320, 16352, 16368, 16376, 16380},
{14336, 15360, 15872, 16128, 16256, 16320, 16352, 16368, 16376},
{12288, 14336, 15360, 15872, 16128, 16256, 16320, 16352, 16368},
{8192, 12288, 14336, 15360, 15872, 16128, 16256, 16320, 16352},
{0, 8192, 12288, 14336, 15360, 15872, 16128, 16256, 16320},
{0, 0, 8192, 12288, 14336, 15360, 15872, 16128, 16256},
{0, 0, 0, 8192, 12288, 14336, 15360, 15872, 16128},
{0, 0, 0, 0, 8192, 12288, 14336, 15360, 15872},
{0, 0, 0, 0, 0, 8192, 12288, 14336, 15360},
{0, 0, 0, 0, 0, 0, 8192, 12288, 14336},
{0, 0, 0, 0, 0, 0, 0, 8192, 12288},
{0, 0, 0, 0, 0, 0, 0, 0, 8192},
{0, 0, 0, 0, 0, 0, 0, 0, 0}, 
{18000}
};

void setup() {
  blinkall(2); // useful for testing
}
void loop() {

  DisplayBitMap();
}
void turnon(int led) {
  int pospin = ledMap[led][0];
  int negpin = ledMap[led][1];
  pinMode (pospin, OUTPUT);
  pinMode (negpin, OUTPUT);
  digitalWrite (pospin, HIGH);
  digitalWrite (negpin, LOW);
}

void alloff() {
  DDRD = B00000010;
  DDRB = B00000000; 
}

void DisplayBitMap()
{
  boolean run=true;
  byte frame = 0;
  byte line = 0;
  unsigned long data;
  while(run == true) {
    for(int i = 0; i < runspeed; i++)     {
      for(line = 0; line < 9; line++)       {
        data = pgm_read_word_near (&BitMap[frame][line]);   // fetch data from program memory
        if (data==18000){
          run=false;
        }
        else for (byte led=0; led<14; ++led) {
          if (data & (1<<led)) {
            turnon((line*14)+led);
            delayMicroseconds(blinkdelay);
            alloff();
          }
          else {
            delayMicroseconds(blinkdelay);
          }
        }

      }
             
    } frame++;  
  }
}

void blinkall(int numblink) {
  alloff();
  for(int n = 0;n < numblink;n++)   {
    for(int i = 0; i < runspeed; i++)     {
      for(int j = 0; j < 126; j++)       {
        turnon(j);
        delayMicroseconds(blinkdelay);
        alloff();
      }
    }
    delay(500);
  }
}


  

