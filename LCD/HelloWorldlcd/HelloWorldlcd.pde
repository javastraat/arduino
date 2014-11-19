/*

pins:

    power supply (GND): 1 (Vss)
    power supply (+5V): 2 (Vdd)
    contrast adjust: 3 (Vo)
    signal for register selection: 4 (RS)
    data read & write: 5 (R/)
    enable signal: 6 (E)
    data bus line: 7 (DB0)
    data bus line: 8 (DB1)
    data bus line: 9 (DB2)
    data bus line: 10 (DB3)
    data bus line: 11 (DB4)
    data bus line: 12 (DB5)
    data bus line: 13 (DB6)
    data bus line: 14 (DB7)
    
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * LCD RS pin to digital pin 12 - 4
 * LCD Enable pin to digital pin 11 - 6
 * LCD D4 pin to digital pin 5 - 11
 * LCD D5 pin to digital pin 4 - 12
 * LCD D6 pin to digital pin 3 - 13
 * LCD D7 pin to digital pin 2 - 14
 * LCD R/W pin to ground  - 5
 * 10K resistor:
 * ends to +5V and ground - 1gnd - 2vdd/+5v
 * wiper to LCD VO pin (pin 3) 
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
}

