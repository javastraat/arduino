/*
Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
library works with all LCD displays that are compatible with the Hitachi HD44780 driver.

This sketch is used for testing the lcd.createChar function.

TITLE: Thumbs Down MOVING

Demo by JO3RI

www.JO3RI.be/arduino
*/

// include the library code:
#include  <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins:
LiquidCrystal lcd(8, 7, 6, 5, 4, 3, 2);

// setting animation speed (delay between frames)
int x = 500;

void setup() {
	// set up the LCD's number of columns and rows:
	lcd.begin(16,2);
	// clear the LCD screen:
	lcd.clear();
}


void loop() {
	// start the Thumbs Down part:
	thumbdownA();
        delay(x);
        lcd.clear();
        thumbdownB();
        delay(x);
        lcd.clear();
}     

void thumbdownA() {
  byte thumb1[8] = {B00001,B00010,B00011,B00100,B00011,B00100,B00011,B00100};
  byte thumb2[8] = {B00011,B00000,B00000,B00000,B00000,B00000,B00000,B00000};
  byte thumb3[8] = {B11110,B00001,B00000,B00000,B00000,B00000,B00000,B00000};
  byte thumb4[8] = {B00000,B11110,B01000,B10001,B10010,B10010,B01100,B00000};
  byte thumb5[8] = {B00000,B10000,B01110,B00010,B00010,B00010,B00010,B00010};
  byte thumb6[8] = {B00110,B01000,B10000,B00000,B00000,B00000,B00000,B00000};
  lcd.createChar(0, thumb1);
  lcd.createChar(1, thumb2);
  lcd.createChar(2, thumb3);
  lcd.createChar(3, thumb4);
  lcd.createChar(4, thumb5);
  lcd.createChar(5, thumb6);
  lcd.setCursor(4,0);
  lcd.write(0);
  lcd.setCursor(4,1);
  lcd.write(1);
  lcd.setCursor(5,0);
  lcd.write(2);
  lcd.setCursor(5,1);
  lcd.write(3);
  lcd.setCursor(6,0);
  lcd.write(4);
  lcd.setCursor(6,1);
  lcd.write(5);
}

void thumbdownB() {
  byte thumb1[8] = {B00000,B00001,B00010,B00011,B00100,B00011,B00100,B00011};
  byte thumb2[8] = {B00100,B00011,B00000,B00000,B00000,B00000,B00000,B00000};
  byte thumb3[8] = {B00000,B11110,B00001,B00000,B00000,B00000,B00000,B00000};
  byte thumb4[8] = {B00000,B00000,B11110,B01000,B10001,B10010,B10010,B01100};
  byte thumb5[8] = {B00000,B00000,B10000,B01110,B00010,B00010,B00010,B00010};
  byte thumb6[8] = {B00010,B00110,B01000,B10000,B00000,B00000,B00000,B00000};
  lcd.createChar(0, thumb1);
  lcd.createChar(1, thumb2);
  lcd.createChar(2, thumb3);
  lcd.createChar(3, thumb4);
  lcd.createChar(4, thumb5);
  lcd.createChar(5, thumb6);
  lcd.setCursor(4,0);
  lcd.write(0);
  lcd.setCursor(4,1);
  lcd.write(1);
  lcd.setCursor(5,0);
  lcd.write(2);
  lcd.setCursor(5,1);
  lcd.write(3);
  lcd.setCursor(6,0);
  lcd.write(4);
  lcd.setCursor(6,1);
  lcd.write(5);
}
