
/*


Pin 	# 	KEY
D0(RX) 	0 	NULL
D1(TX) 	1 	NULL
D2 	2 	KEY A
D3 	3 	KEY B
D4 	4 	KEY C
D5 	5 	KEY D
D6 	6 	KEY E
D7 	7 	KEY F
D8 	8 	KEY Down
Nokoia 5110 LCD
Pin 	# 	KEY
D9 	9 	SCLK
D10 	10 	CSN
D11 	11 	D/C
D12 	12 	RST
D13 	13 	SCE
nRF24L01
Pin 	# 	KEY
D9 	9 	SCE
D10 	10 	CSN
D11 	11 	SCK
D12 	12 	MOSI
D13 	13 	MISO 



*/

/*********************************************************************
**  Device: Joystick  -- Greedy Freak v1.0                          **
**  File:   EF_Joystick_v2.0_Game.pde                               **
**								    **
**  Created by ElecFreaks Robi.W /25 Nov 2011                       **
**                                                                  **
**  Description:                                                    **
**  This file is a sample code for your reference.                  **
**  A Greedy Freak Simple Game base on LCD5110_Graph library. And   **
**  the Hardware is base on Freaduino328 and Nokia 5110 LCD. The    **
**  Joystick v2.0 with Nokia5110 is not support standard Arduino    **
**  board Because of standard Arduino board 3.3v just supply 50mA,  **
**  which can't support Nokia5110 backlight need.However Freaduino ** 
**  support 800mA far more than Nokia5110's demand.                 **
**                                                                  ** 
**  This demo code is free software; you can redistribute it and/or **
**  modify it. If you have any achievemen, please share with us     **
**  Copyright (C) 2011 ElecFreaks Corp.                     	    **
**  www.elecfreaks.com                                              **
*********************************************************************/
 
#include <LCD5110_Graph.h>
#include <avr/pgmspace.h>

LCD5110 myGLCD(9,10,11,12,13);
 
extern uint8_t SmallFont[];
 
uint8_t pacman1[] PROGMEM={
0x80, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7E, 0x3E, 0x1C,   // 0x0010 (16) pixels
0x0C, 0x00, 0x00, 0x00, 0x1F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9,   // 0x0020 (32) pixels
0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x0F,   // 0x0030 (48) pixels
0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x03, 0x03, 0x00, 0x00, 0x00, 
};
 
uint8_t pacman2[] PROGMEM={
0x80, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0x7C,   // 0x0010 (16) pixels
0x7C, 0x38, 0x20, 0x00, 0x1F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9,   // 0x0020 (32) pixels
0xF9, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x0F,   // 0x0030 (48) pixels
0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 
};
 
uint8_t pacman3[] PROGMEM={
0x80, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC,   // 0x0010 (16) pixels
0xF8, 0xF0, 0xE0, 0x80, 0x1F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0020 (32) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFB, 0xF9, 0x79, 0x19, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x0F,   // 0x0030 (48) pixels
0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 
};
 
uint8_t pill[] PROGMEM={
0x0E, 0x1F, 0x1F, 0x1F, 0x0E, 
};
#define Width  84
#define Hight  48
#define Range  12

int FirstShotX , FirstShotY;
int PointX, PointY;
int delaytime;
 
void setup()
{
  /* Init LCD5110_Graph library */
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  randomSeed(analogRead(0));
 
  /* Record Joystick corrected coordinates */
  FirstShotX = analogRead(A0);
  FirstShotY = analogRead(A1);
 
  /* Black specks the initial coordinates */
  PointX = 75;
  PointY = 20;
 
  /* Refresh time */
  delaytime = 50;
 
  /* Start display */
  myGLCD.print("-=Einstein=-", CENTER, 0);
  myGLCD.print("Joystick v2.0", CENTER, 20);
  myGLCD.print("Greedy Freak", CENTER, 40);
  myGLCD.update();
  delay(3000);
 
  /* Init Serial port */
  Serial.begin(115200);
}
 
void loop()
{
  //int pacy=random(0, 42);
  uint8_t* bm;
 
  for (int i=-20; i<84; i++)
  {
    /* Clear LCD */
    myGLCD.clrScr();     
 
    /* Refresh Greedy Freak Bitmap */
    switch(((i+20)/3) % 4)
    {
      case 0: bm=pacman1;
              break;
      case 1: bm=pacman2;
              break;
      case 2: bm=pacman3;
              break;
      case 3: bm=pacman2;
              break;
    }
    /*You can change the coefficient such as 0.08, which decide X-axis Range*/
    int sensorValueX = (analogRead(A0) - FirstShotX)*0.1 + 32; 
    /*You can change the coefficient such as 0.04, which decide Y-axis Range*/       
    int sensorValueY = (FirstShotY - analogRead(A1))*0.06 + 14;         
 
    myGLCD.drawBitmap(sensorValueX, sensorValueY, bm, 20, 20);
    /* Once the Greedy Freak be close to black specks, random another X and Y */
TX:                                                                
    if( (sensorValueX-5 <= PointX && PointX <= sensorValueX+15) && (sensorValueY-3<= PointY && PointY <= sensorValueY + 20 ))
    {
       PointX = random(0, 80);
       PointY = random(0, 43);  
       goto TX;
    }
    else
      myGLCD.drawBitmap(PointX, PointY, pill, 5, 5);
 
    myGLCD.update();        //update and display the Bitmap
 
    int i, someInt, flag = 0;
    for(i=2; i<9; i++)
    {
      someInt = digitalRead(i);        //scan the KEY
      if(someInt == 0)
      {  
        flag =1;
        break;
      }
     }
     if(flag == 1)
     {    
       Serial.println(i); 
       switch(i)
       {
         case 2: Serial.println("--------> Button A"); 
                 delaytime = 50;
                 break;
         case 3: Serial.println("--------> Button B"); 
                 delaytime = 5;
                 break;
         case 4: Serial.println("--------> Button C"); 
                 delaytime = 50;
                 break;
         case 5: Serial.println("--------> Button D"); 
                 delaytime = 5;
                 break;
         case 6: Serial.println("--------> Button E"); break;
         case 7: Serial.println("--------> Button F"); break;
         case 8: Serial.println("--------> Button KEY"); break;
         default: break;
       }
       flag=0;
     }
    delay(delaytime);
  }
}


