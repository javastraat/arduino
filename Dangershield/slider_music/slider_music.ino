
///////////////////////////////////////////////////////
// Danger Shield Sequencer
//   Arthur: mhd
//
// The goal of this sketch is to use the sliders on
// the Danger Shield to change the chords in the
// sequencer. There should also be the option to switch
// out the function of the first slider to control
// the BPM of the melody/sequence
//
/////
// Most of "#define"s, ledCharSet[], and sevenSeg() 
// taken from:
//
// Danger Shield Example Sketch
// Copyright (c) 2010 SparkFun Electronics.  All right reserved.
// Written by Chris Taylor
//
/////
// pitches.h taken from:
// http://arduino.cc/en/Reference/Tone
///////////////////////////////////////////////////////
// v000:
// 
///////////////////////////////////////////////////////

#include "pitches.h"

// Shift register bit values to display 0-9 on the seven-segment display
const byte ledCharSet[11] = 
{
  B00111111,//0
  B00000110,//1
  B01011011,//2
  B01001111,//3
  B01100110,//4
  B01101101,//5
  B01111101,//6
  B00000111,//7
  B01111111,//8
  B01101111,//9
  B00011011 //some symbol
};

// Global variables
int val   = 0;
int state = 0;
int x     = 0;
int i     = 0;

// Digital Pin definitions
#define SLIDER1   2
#define SLIDER2   1
#define SLIDER3   0

#define BUZZER    3

#define LED1      5
#define LED2      6

#define DATA      4
#define LATCH     7
#define CLOCK     8

#define BUTTON1  10
#define BUTTON2  11
#define BUTTON3  12

// Analog Read Defintions
#define LIGHT     3
#define TEMP      4
#define KNOCK     5

#define DELAYTIME 125

int slider[3] =
{ SLIDER1, SLIDER2, SLIDER3 };

int printNum[3] =
{ 1, 2, 3 };


void setup()
{
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA,OUTPUT);

}

void loop()
{
  for(int j=0; j<3; j++)
  {
    sevenSegPrint(printNum[j]);
    val = analogRead(slider[j]);
    val = (1023-val) >> 5;
  
    tone(BUZZER, cMaj[val]);
    delay(DELAYTIME);
    
    tone(BUZZER, cMaj[val+4]);
    delay(DELAYTIME);
    
    tone(BUZZER, cMaj[val+2]);
    delay(DELAYTIME);
    
    tone(BUZZER, cMaj[val]);
    delay(DELAYTIME);
    
    tone(BUZZER, cMaj[val+4]);
    delay(DELAYTIME);
    
    tone(BUZZER, cMaj[val+2]);
    delay(DELAYTIME);
    
    tone(BUZZER, cMaj[val+1]);
    delay(DELAYTIME);
    
    tone(BUZZER, cMaj[val+4]);
    delay(DELAYTIME);  
    
  }// for j
  
}//loop



void sevenSegPrint(int num)
{
  //sanitize input for ledCharSet[]
  if(num > 10)
    num=10;
  else
  if(num < 0)
    num=10;
    
  digitalWrite(LATCH,LOW);
  shiftOut(DATA,CLOCK,MSBFIRST,~(ledCharSet[num]));
  digitalWrite(LATCH,HIGH);
  delay(5);
}//sevenSegPrint

