/*

Example 24.1 - LCD demonstration

http://tronixstuff.com/tutorials > Chapter 24  CC by-sa-nc liquidCrystal library originally added 18 Apr 2008 by David A. Mellis

library modified 5 Jul 2009   by Limor Fried (http://www.ladyada.net, http://www.adafruit.com)

*/

#include <LiquidCrystal.h> // we need this library for the LCD commands // initialize the library with the numbers of the interface pins

LiquidCrystal lcd(8, 7, 6, 5, 4, 3, 2); // define our LCD and which pins to user
int d = 3000; // used for display delay

byte a[8] = {  B00000,  B00000,  B00000,  B00100,  B00100,    B00000,  B00000,  B00000}; byte b[8] = {  B00000,  B00000,  B10001,  B10001,  B10001,  B10001,  B00000,  B00000}; byte c[8] = {  B11111,  B10001,  B10001,  B10001,  B10001,  B10001,  B10001,  B11111};   void setup()

{

lcd.createChar(0,a); // define our characters into the sketch as variables

lcd.createChar(1,b);

lcd.createChar(2,c);

lcd.begin(16, 2); // need to specify how many columns and rows are in the LCD unit

lcd.clear();      // this clears the LCD. You can use this at any time

}

void loop()

{

for (int z=0; z<16; z++)

{

lcd.setCursor(z,0);

lcd.write(0);  // write the first character

delay(250);

lcd.setCursor(z,0);

lcd.write(1);  // write the second character

delay(250);

lcd.setCursor(z,0);

lcd.write(2);  // write the third character

delay(250);

lcd.setCursor(z,0);

lcd.write(1); // write the second character

delay(250);

lcd.setCursor(z,0);

lcd.write(0);  // write the first character

delay(250);

}

delay(1000);

lcd.clear();

}
