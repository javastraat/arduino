#include <LedControl.h>

//We always have to include the library
//#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 10 is connected to LOAD 
 pin 11 is connected to the CLK
 We have only a single MAX72XX.
 */
 
  #define DinPin 43
 #define CsPin 45
 #define ClkPin 47
 #define chipno 0
 #define chips 8
 
//LedControl lc=LedControl(12,10,11,9);
LedControl lc=LedControl(DinPin,ClkPin,CsPin,chips); 
/* we always wait a bit between updates of the display */
unsigned long delaytime=1000;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  for(int i=0;i<4;i++)
 { 
  lc.shutdown(i,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(i,8);
  /* and clear the display */
  lc.clearDisplay(i);
 }
  
  writeArduinoOnMatrix();
}

/*
 This method will display the characters for the
 word "Arduino" one after the other on the matrix. 
 (you need at least 5x7 leds to see the whole chars)
 */
void writeArduinoOnMatrix() {
  /* here is the data for the characters */
  byte a[8]={B00000000,
             B00000000,
             B01000010,
             B01111110,
             B01000010,
             B00000000,
             B00000000, 
             B00000000};
  byte b[8]={B00000000,
             B00011100,
             B00100010,
             B01000100,
             B01000100,
             B00100010,
             B00011100,
             B00000000};
  byte c[8]={B00000000,
             B00000010,
             B00000010,
             B01111110,
             B00000010,
             B00000010,
             B00000000,
             B00000000};
  byte d[8]={B00000000,
             B00111100,
             B01000010,
             B01000010,
             B01000010,
             B00100100,
             B00000000,
             B00000000};
  byte e[8]={B00000000,
             B00000010,
             B00000010,
             B01111110,
             B00000010,
             B00000010,
             B00000000,
             B00000000};
  byte f[8]={B00000000,
             B01111110,
             B00000100,
             B00011000,
             B00000100,
             B01111110,
             B00000000,
             B00000000};
             
  /* now display them one by one with a small delay */
//  lc.setCharFont5X7(0, 0, 'H');
//   lc.setCharFont5X7(1, 0, 'e');
  lc.setStringFont5X7(0,0,"Hello World!",6); //FontWidth=6
#if 0  
  lc.setRow(0,0,a[0]);
  lc.setRow(0,1,a[1]);
  lc.setRow(0,2,a[2]);
  lc.setRow(0,3,a[3]);
  lc.setRow(0,4,a[4]);
  lc.setRow(0,5,a[5]);
  lc.setRow(0,6,a[6]);
  lc.setRow(0,7,a[7]);
   lc.setRow(1,0,b[0]);
  lc.setRow(1,1,b[1]);
  lc.setRow(1,2,b[2]);
  lc.setRow(1,3,b[3]);
  lc.setRow(1,4,b[4]);
  lc.setRow(1,5,b[5]);
  lc.setRow(1,6,b[6]);
  lc.setRow(1,7,b[7]);
#endif
  Serial.begin(9600);
  Serial.println(lc.status[0],BIN);
  Serial.println(lc.status[1],BIN);
  Serial.println(lc.status[2],BIN);
  Serial.println(lc.status[3],BIN);
  Serial.println(lc.status[4],BIN);
  Serial.println(lc.status[5],BIN);
  Serial.println(lc.status[6],BIN);
  Serial.println(lc.status[7],BIN);
  
//  char *a=new char(5);
}

void LShift()
{
  byte firststatus=lc.status[0];
  lc.setRow(0,0,lc.status[1]);
  lc.setRow(0,1,lc.status[2]);
  lc.setRow(0,2,lc.status[3]);
  lc.setRow(0,3,lc.status[4]);
  lc.setRow(0,4,lc.status[5]);
  lc.setRow(0,5,lc.status[6]);
  lc.setRow(0,6,lc.status[7]);
   lc.setRow(0,7,lc.status[8]);
   lc.setRow(1,0,lc.status[9]);
  lc.setRow(1,1,lc.status[10]);
  lc.setRow(1,2,lc.status[11]);
  lc.setRow(1,3,lc.status[12]);
  lc.setRow(1,4,lc.status[13]);
  lc.setRow(1,5,lc.status[14]);
  lc.setRow(1,6,lc.status[15]);
  lc.setRow(1,7,firststatus);
  delay(100);
  
}


void loop() { 
 //LShift();//  writeArduinoOnMatrix();
 lc.LeftRotate(1);
 //delay(100);
}
