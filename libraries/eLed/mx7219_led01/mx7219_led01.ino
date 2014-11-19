#include "LedControl.h"
/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
 #define DinPin 43
 #define CsPin 45
 #define ClkPin 47
 #define chipno 0 
LedControl lc=LedControl(DinPin,ClkPin,CsPin,chipno); 

/* we always wait a bit between updates of the display */
unsigned long delaytime=2000;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
  Serial.begin(9600);
  Serial.println("Program Start Here");
}

void loop() { 
  writeArduinoOnMatrix();
  //rows();
 // columns();
//  single();
}

/*
 This method will display the characters for the
 word "Arduino" one after the other on the matrix. 
 (you need at least 5x7 leds to see the whole chars)
 */
void writeArduinoOnMatrix() {
  /* here is the data for the characters */
  byte a[8]={B00011000,
             B00100100,  
             B01000010,  
             B10000001,  
             B11111111,
             B10000001,
             B10000001,
             B00000000};
             

             
  /* now display them one by one with a small delay */
  showhar(&a[0]);
   Serial.println("Now print (A)");
  delay(delaytime);

 
}
void showhar(byte *chr)
{
 int i = 0 ;
   clearchar();
   //lc.clearDisplay(0) ;
 for(i = 0 ; i <8 ; i++)
 {
   lc.setColumn(0,i,*(chr+i));
 } 
 
}

void clearchar()
{
 int i = 0 ;
 for(i = 0 ; i <8 ; i++)
 {
   lc.setColumn(0,i,0x00);
 } 
 
}

/*
  This fushoion lights ua some Leds in a row.
 The pattern will be repeatd on every row.
 The pattern will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.
 */
void rows() {
  for(int row=0;row<8;row++) {
    delay(delaytime);
    lc.setRow(0,row,B10100000);
    delay(delaytime);
    lc.setRow(0,row,(byte)0);
    for(int i=0;i<row;i++) {
      delay(delaytime);
      lc.setRow(0,row,B10100000);
      delay(delaytime);
      lc.setRow(0,row,(byte)0);
    }
  }
}

/*
  This function lights up a some Leds in a column.
 The pattern will be repeated on every column.
 The pattern will blink along with the column-number.
 column number 4 (index==3) will blink 4 times etc.
 */
void columns() {
  for(int col=0;col<8;col++) {
    delay(delaytime);
    lc.setColumn(0,col,B10100000);
    delay(delaytime);
    lc.setColumn(0,col,(byte)0);
    for(int i=0;i<col;i++) {
      delay(delaytime);
      lc.setColumn(0,col,B10100000);
      delay(delaytime);
      lc.setColumn(0,col,(byte)0);
    }
  }
}

/* 
 This function will light up every Led on the matrix.
 The led will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.
 */
void single() {
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      delay(delaytime);
      lc.setLed(0,row,col,true);
      delay(delaytime);
      for(int i=0;i<col;i++) {
        lc.setLed(0,row,col,false);
        delay(delaytime);
        lc.setLed(0,row,col,true);
        delay(delaytime);
      }
    }
  }
}


