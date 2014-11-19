/*
Name: Scrolling Text
Author: PoisonMondo
Date: 1/25/2014
Version: 2.0
*/


#include <Colorduino_GFX.h>
#include <Adafruit_GFX.h>


//===============================//
/*This section defines the number
  of characters for your message
  Possible values: 4, 7, 11, 15
  Default: 11 
*/
int characters = 15;

//==============================//
//===============================//
/*This section defines the display
Number of your colorduino(used for
large displays)so for two displays
set the first deisplay to '1' and
the second to '2' and connect them
together
*/
int displaynumber = 1;

//==============================//
int startdelay = 0;

//Create a new colorduino instance
ColorduinoPanel Colorduino;
//Variable to control Movement
int x = 7;
int x2 = 7;
int x3 = 7;
int x4 = 7;
int x5 = 7;
int x6 = 7;
int x7 = 7;
int x8 = 7;
int x9 = 7;
int x10 = 7;
int x11 = 7;
int x12 = 7;
int x13 = 7;
int x14 = 7;
int x15 = 7;
int x16 = 7;
int x17 = 7;
int x18 = 7;
int x19 = 7;
int x20 = 7;
int x21 = 7;
int x22 = 7;
int x23 = 7;
//Addjust to change scroll speed
int t = 200;
//Scrolling Letters
char scroll_1 = 'H';
char scroll_2 = 'E';
char scroll_3 = 'L';
char scroll_4 = 'L';
char scroll_5 = 'O'; 
char scroll_6 = 3;// Love Heart
char scroll_7 = 'H';
char scroll_8 = 'O';
char scroll_9 = 'W';
char scroll_10 = 32;// Space
char scroll_11 = 'A';
char scroll_12 = 'R';
char scroll_13 = 'E';
char scroll_14 = 32;// Space
char scroll_15 = 'U';
char scroll_16 = 32;//NOT IMPLEMENTED
char scroll_17 = 32;//NOT IMPLEMENTED
char scroll_18 = 32;//NOT IMPLEMENTED
char scroll_19 = 32;//NOT IMPLEMENTED
char scroll_20 = 32;//NOT IMPLEMENTED
char scroll_21 = 32;//NOT IMPLEMENTED
char scroll_22 = 32;//NOT IMPLEMENTED
char scroll_23 = 32;//NOT IMPLEMENTED
char scroll_24 = 32;//NOT IMPLEMENTED

//Text Colour - Colorduino.color(R, G, B);
GFX_Color_t textcolor = Colorduino.color(0, 255, 0); //Scroll 1
GFX_Color_t textcolor1 = Colorduino.color(0, 255, 0); //Scroll 2
GFX_Color_t textcolor2 = Colorduino.color(0, 255, 0);//Scroll 3
GFX_Color_t textcolor3 = Colorduino.color(0, 255, 0);//Scroll 4
GFX_Color_t textcolor4 = Colorduino.color(255, 20, 147); //Scroll 5
GFX_Color_t textcolor5 = Colorduino.color(0, 191, 255);//Scroll 6
GFX_Color_t textcolor6 = Colorduino.color(0, 191, 255);//Scroll 7
GFX_Color_t textcolor7 = Colorduino.color(0, 191, 255);//Scroll 8
GFX_Color_t textcolor8 = Colorduino.color(0, 191, 255);//Scroll 9
GFX_Color_t textcolor9 = Colorduino.color(0, 191, 255);//Scroll 10
GFX_Color_t textcolor10 = Colorduino.color(0, 191, 255);//Scroll 11
GFX_Color_t textcolor11 = Colorduino.color(255, 0, 150);//Scroll 11
GFX_Color_t textcolor12 = Colorduino.color(255, 0, 150);//Scroll 11
GFX_Color_t textcolor13 = Colorduino.color(255, 0, 150);//Scroll 11
GFX_Color_t textcolor14 = Colorduino.color(255, 0, 150);//Scroll 11
GFX_Color_t textcolor15 = Colorduino.color(255, 0, 150);//Scroll 11
GFX_Color_t textcolor16 = Colorduino.color(255, 0, 150);//Scroll 11
GFX_Color_t textcolor17 = Colorduino.color(255, 0, 150);//Scroll 11
GFX_Color_t textcolor18 = Colorduino.color(255, 0, 150);//Scroll 11
GFX_Color_t textcolor19 = Colorduino.color(255, 0, 150);//Scroll 11
GFX_Color_t textcolor20 = Colorduino.color(255, 0, 150);//Scroll 11
GFX_Color_t textcolor21 = Colorduino.color(255, 0, 150);//Scroll 11
GFX_Color_t textcolor22 = Colorduino.color(255, 0, 150);//Scroll 11
GFX_Color_t textcolor23 = Colorduino.color(255, 0, 150);//Scroll 11


void setup() {
  //Initiate the Colorduino
  startdelay = t*8*displaynumber;
  Colorduino.init();
  // Set white balance
  Colorduino.setWhiteBalance(36, 63, 63);
  delay(startdelay);
}

void loop() {
if (characters == 4) {
  //4 Character Code
   while (x >= -6) { 
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x, 1, scroll_1, textcolor, Colorduino.color(0, 0, 0), 1);
      x--;
      if (x <= 0 ) {
        Colorduino.drawChar(x2, 1, scroll_2, textcolor1, Colorduino.color(0, 0, 0), 1);
        x2--;
      }
      if (x2 <=0 ) {
        Colorduino.drawChar(x3, 1, scroll_3, textcolor2, Colorduino.color(0, 0, 0), 1);
        x3--;
      }
      Colorduino.swapBuffers(true);
      delay(t);
      
 } 
 while (x3 >= -12) {
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x3, 1, scroll_3, textcolor2, Colorduino.color(0, 0, 0), 1);
      x3--;
      if (x2 >= -6) {
       Colorduino.drawChar(x2, 1, scroll_2, textcolor1, Colorduino.color(0, 0, 0), 1);
        x2--; 
      }
      
      if (x3 <= 0 ) {
        Colorduino.drawChar(x4, 1, scroll_4, textcolor3, Colorduino.color(0, 0, 0), 1);
        x4--; 
      }
      
      Colorduino.swapBuffers(true);
      delay(t);
 }

}
//8 Characters
else if (characters == 7) {
  while (x >= -6) { 
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x, 1, scroll_1, textcolor, Colorduino.color(0, 0, 0), 1);
      x--;
      if (x <= 0 ) {
        Colorduino.drawChar(x2, 1, scroll_2, textcolor1, Colorduino.color(0, 0, 0), 1);
        x2--;
      }
      if (x2 <=0 ) {
        Colorduino.drawChar(x3, 1, scroll_3, textcolor2, Colorduino.color(0, 0, 0), 1);
        x3--;
      }
      Colorduino.swapBuffers(true);
      delay(t);
      
 } 
 while (x3 >= -6) {
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x3, 1, scroll_3, textcolor2, Colorduino.color(0, 0, 0), 1);
      x3--;
      if (x2 >= -6) {
       Colorduino.drawChar(x2, 1, scroll_2, textcolor1, Colorduino.color(0, 0, 0), 1);
        x2--; 
      }
      
      if (x3 <= 0 ) {
        Colorduino.drawChar(x4, 1, scroll_4, textcolor3, Colorduino.color(0, 0, 0), 1);
        x4--; 
      }
      if (x4 <= 0 ) {
        Colorduino.drawChar(x5, 1, scroll_5, textcolor4, Colorduino.color(0, 0, 0), 1);
        x5--; 
      }
      
      Colorduino.swapBuffers(true);
      delay(t);
 }
 while (x5 >= -6) {
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x5, 1, scroll_5, textcolor4, Colorduino.color(0, 0, 0), 1);
      x5--;
      if (x4 >= -6) {
       Colorduino.drawChar(x4, 1, scroll_4, textcolor3, Colorduino.color(0, 0, 0), 1);
        x4--; 
      }
      if (x5 <= 0 ) {
        Colorduino.drawChar(x6, 1, scroll_6, textcolor5, Colorduino.color(0, 0, 0), 1);
        x6--; 
      }
      if (x6 <= 0 ) {
        Colorduino.drawChar(x7, 1, scroll_7, textcolor6, Colorduino.color(0, 0, 0), 1);
        x7--;
      }
      Colorduino.swapBuffers(true);
      delay(t);
 }
 while (x7 >= -6) {
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x7, 1, scroll_7, textcolor6, Colorduino.color(0, 0, 0), 1);
      x7--;
      if (x6 >= -6) {
        Colorduino.drawChar(x6, 1, scroll_6, textcolor5, Colorduino.color(0, 0, 0), 1);
        x6--;
      }
      Colorduino.swapBuffers(true);
      delay(t);
}
}
else if (characters == 11) {
//11 Character code
 while (x >= -6) { 
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x, 1, scroll_1, textcolor, Colorduino.color(0, 0, 0), 1);
      x--;
      if (x <= 0 ) {
        Colorduino.drawChar(x2, 1, scroll_2, textcolor1, Colorduino.color(0, 0, 0), 1);
        x2--;
      }
      if (x2 <=0 ) {
        Colorduino.drawChar(x3, 1, scroll_3, textcolor2, Colorduino.color(0, 0, 0), 1);
        x3--;
      }
      Colorduino.swapBuffers(true);
      delay(t);
      
 } 
 while (x3 >= -6) {
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x3, 1, scroll_3, textcolor2, Colorduino.color(0, 0, 0), 1);
      x3--;
      if (x2 >= -6) {
       Colorduino.drawChar(x2, 1, scroll_2, textcolor1, Colorduino.color(0, 0, 0), 1);
        x2--; 
      }
      
      if (x3 <= 0 ) {
        Colorduino.drawChar(x4, 1, scroll_4, textcolor3, Colorduino.color(0, 0, 0), 1);
        x4--; 
      }
      if (x4 <= 0 ) {
        Colorduino.drawChar(x5, 1, scroll_5, textcolor4, Colorduino.color(0, 0, 0), 1);
        x5--; 
      }
      
      Colorduino.swapBuffers(true);
      delay(t);
 }
 while (x5 >= -6) {
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x5, 1, scroll_5, textcolor4, Colorduino.color(0, 0, 0), 1);
      x5--;
      if (x4 >= -6) {
       Colorduino.drawChar(x4, 1, scroll_4, textcolor3, Colorduino.color(0, 0, 0), 1);
        x4--; 
      }
      if (x5 <= 0 ) {
        Colorduino.drawChar(x6, 1, scroll_6, textcolor5, Colorduino.color(0, 0, 0), 1);
        x6--; 
      }
      if (x6 <= 0 ) {
        Colorduino.drawChar(x7, 1, scroll_7, textcolor6, Colorduino.color(0, 0, 0), 1);
        x7--;
      }
      Colorduino.swapBuffers(true);
      delay(t);
 }
 while (x7 >= -6) {
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x7, 1, scroll_7, textcolor6, Colorduino.color(0, 0, 0), 1);
      x7--;
      if (x6 >= -6) {
        Colorduino.drawChar(x6, 1, scroll_6, textcolor5, Colorduino.color(0, 0, 0), 1);
        x6--;
      }
      if (x7 <= 0 ) {
        Colorduino.drawChar(x8, 1, scroll_8, textcolor7, Colorduino.color(0, 0, 0), 1);
        x8--;
      }
      if (x8 <= 0 ) {
        Colorduino.drawChar(x9, 1, scroll_9, textcolor8, Colorduino.color(0, 0, 0), 1);
        x9--;
      }
      Colorduino.swapBuffers(true);
      delay(t);
 }
 while (x9 >= -6) {
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x9, 1, scroll_9, textcolor8, Colorduino.color(0, 0, 0), 1);
      x9--;
      if (x8 >= -6) {
        Colorduino.drawChar(x8, 1, scroll_8, textcolor7, Colorduino.color(0, 0, 0), 1);
        x8--;
      }
      if (x9 <= 0 ) {
        Colorduino.drawChar(x10, 1, scroll_10, textcolor9, Colorduino.color(0, 0, 0), 1);
        x10--;
      }
      if (x10 <= 0 ) {
        Colorduino.drawChar(x11, 1, scroll_11, textcolor10, Colorduino.color(0, 0, 0), 1);
        x11--;
      }
      Colorduino.swapBuffers(true);
      delay(t);
 }
 while (x11 >= -6) {
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x11, 1, scroll_11, textcolor10, Colorduino.color(0, 0, 0), 1);
      x11--;
      if (x10 >= -6) {
        Colorduino.drawChar(x10, 1, scroll_10, textcolor9, Colorduino.color(0, 0, 0), 1);
        x10--;
      }
      Colorduino.swapBuffers(true);
      delay(t);
 }
}
//15 characters
else if (characters == 15) {
   while (x >= -6) { 
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x, 1, scroll_1, textcolor, Colorduino.color(0, 0, 0), 1);
      x--;
      if (x <= 0 ) {
        Colorduino.drawChar(x2, 1, scroll_2, textcolor1, Colorduino.color(0, 0, 0), 1);
        x2--;
      }
      if (x2 <=0 ) {
        Colorduino.drawChar(x3, 1, scroll_3, textcolor2, Colorduino.color(0, 0, 0), 1);
        x3--;
      }
      Colorduino.swapBuffers(true);
      delay(t);
      
 } 
 while (x3 >= -6) {
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x3, 1, scroll_3, textcolor2, Colorduino.color(0, 0, 0), 1);
      x3--;
      if (x2 >= -6) {
       Colorduino.drawChar(x2, 1, scroll_2, textcolor1, Colorduino.color(0, 0, 0), 1);
        x2--; 
      }
      
      if (x3 <= 0 ) {
        Colorduino.drawChar(x4, 1, scroll_4, textcolor3, Colorduino.color(0, 0, 0), 1);
        x4--; 
      }
      if (x4 <= 0 ) {
        Colorduino.drawChar(x5, 1, scroll_5, textcolor4, Colorduino.color(0, 0, 0), 1);
        x5--; 
      }
      
      Colorduino.swapBuffers(true);
      delay(t);
 }
 while (x5 >= -6) {
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x5, 1, scroll_5, textcolor4, Colorduino.color(0, 0, 0), 1);
      x5--;
      if (x4 >= -6) {
       Colorduino.drawChar(x4, 1, scroll_4, textcolor3, Colorduino.color(0, 0, 0), 1);
        x4--; 
      }
      if (x5 <= 0 ) {
        Colorduino.drawChar(x6, 1, scroll_6, textcolor5, Colorduino.color(0, 0, 0), 1);
        x6--; 
      }
      if (x6 <= 0 ) {
        Colorduino.drawChar(x7, 1, scroll_7, textcolor6, Colorduino.color(0, 0, 0), 1);
        x7--;
      }
      Colorduino.swapBuffers(true);
      delay(t);
 }
 while (x7 >= -6) {
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x7, 1, scroll_7, textcolor6, Colorduino.color(0, 0, 0), 1);
      x7--;
      if (x6 >= -6) {
        Colorduino.drawChar(x6, 1, scroll_6, textcolor5, Colorduino.color(0, 0, 0), 1);
        x6--;
      }
      if (x7 <= 0 ) {
        Colorduino.drawChar(x8, 1, scroll_8, textcolor7, Colorduino.color(0, 0, 0), 1);
        x8--;
      }
      if (x8 <= 0 ) {
        Colorduino.drawChar(x9, 1, scroll_9, textcolor8, Colorduino.color(0, 0, 0), 1);
        x9--;
      }
      Colorduino.swapBuffers(true);
      delay(t);
 }
 while (x9 >= -6) {
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x9, 1, scroll_9, textcolor8, Colorduino.color(0, 0, 0), 1);
      x9--;
      if (x8 >= -6) {
        Colorduino.drawChar(x8, 1, scroll_8, textcolor7, Colorduino.color(0, 0, 0), 1);
        x8--;
      }
      if (x9 <= 0 ) {
        Colorduino.drawChar(x10, 1, scroll_10, textcolor9, Colorduino.color(0, 0, 0), 1);
        x10--;
      }
      if (x10 <= 0 ) {
        Colorduino.drawChar(x11, 1, scroll_11, textcolor10, Colorduino.color(0, 0, 0), 1);
        x11--;
      }
      Colorduino.swapBuffers(true);
      delay(t);
 }
 while (x11 >= -6) {
      Colorduino.fillColor(Colorduino.color(0, 0, 0));
      Colorduino.drawChar(x11, 1, scroll_11, textcolor10, Colorduino.color(0, 0, 0), 1);
      x11--;
      if (x10 >= -6) {
        Colorduino.drawChar(x10, 1, scroll_10, textcolor9, Colorduino.color(0, 0, 0), 1);
        x10--;
      }
      if (x11 <= 0) {
        Colorduino.drawChar(x12, 1, scroll_12, textcolor11, Colorduino.color(0, 0, 0), 1);
        x12--;
      }
      if (x12 <= 0) {
        Colorduino.drawChar(x13, 1, scroll_13, textcolor12, Colorduino.color(0, 0, 0), 1);
        x13--;
      }
      Colorduino.swapBuffers(true);
      delay(t);
 }
 while (x13 >= -6) {
   Colorduino.fillColor(Colorduino.color(0, 0, 0));
   Colorduino.drawChar(x13, 1, scroll_13, textcolor12, Colorduino.color(0, 0, 0), 1);
   x13--;
   if (x12 >= -6) {
     Colorduino.drawChar(x12, 1, scroll_12, textcolor11, Colorduino.color(0, 0, 0), 1);
     x12--;
   }
   if (x13 <= 0) {
     Colorduino.drawChar(x14, 1, scroll_14, textcolor13, Colorduino.color(0, 0, 0), 1);
     x14--;
   }
   if (x14 <= 0) {
     Colorduino.drawChar(x15, 1, scroll_15, textcolor14, Colorduino.color(0, 0, 0), 1);
     x15--;
   }
   Colorduino.swapBuffers(true);
   delay(t);
 }
 while (x15 >= -6) {
   Colorduino.fillColor(Colorduino.color(0, 0, 0));
   Colorduino.drawChar(x15, 1, scroll_15, textcolor14, Colorduino.color(0, 0, 0), 1);
   x15--;
   if (x14 >= -6) {
     Colorduino.drawChar(x14, 1, scroll_14, textcolor13, Colorduino.color(0, 0, 0), 1);
     x14--;
   }
 Colorduino.swapBuffers(true);
 delay(t);
 }
}
 //Reset Variables
 x = 7;
 x2 = 7;
 x3 = 7;
 x4 = 7;
 x5 = 7;
 x6 = 7;
 x7 = 7;
 x8 = 7;
 x9 = 7;
 x10 = 7;
 x11 = 7;
 x12 = 7;
 x13 = 7;
 x14 = 7;
 x15 = 7;
 x16 = 7;
 x17 = 7;
 x18 = 7;
 x19 = 7;
 x20 = 7;
 x21 = 7;
 x22 = 7;
 x23 = 7;
}

