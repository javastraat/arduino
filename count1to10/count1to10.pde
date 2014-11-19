#define LATCH 7
#define CLOCK 8
#define DATA 4

// 7 LED Display
//
// B hgfedcba
//    a
//  f   b
//    g
//  e   c
//    d   h
//
// 0 =  B00111111
// only g and h are off
// 0. =  B10111111
// h on


const byte ledCharSet[10] = {

  B10111111,B00000110,B01011011,B01001111,B01100110,B01101101,B01111101,B00000111,B01111111,B01101111
};


// Global variables
int i = 0;
void setup()
{

  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA,OUTPUT);
}
void loop()
{
  digitalWrite(LATCH,LOW);
  shiftOut(DATA,CLOCK,MSBFIRST,(~ledCharSet[i]));
  digitalWrite(LATCH,HIGH);
  i++;
  if(i==10){
    i = 0;
  }
  delay(500);
}

