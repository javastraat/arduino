/*
Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
library works with all LCD displays that are compatible with the Hitachi HD44780 driver.

This sketch is used for testing the lcd.createChar function.

Demo by JO3RI


*/

// include the library code:
#include  <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins:
LiquidCrystal lcd(8, 7, 6, 5, 4, 3, 2);
// set speed for shift upper - lower part:
int x=1000;

void setup() {
	// set up the LCD's number of columns and rows:
	lcd.begin(16,2);
	// clear the LCD screen:
	lcd.clear();
}


void loop() {
	// start the arduinologo part:
	arduinologo();
}     

//the arduinologo logo itself
void arduinologo(){
	upperpart();
        lcd.setCursor(4,0);
	lcd.write(0);
	lcd.setCursor(5,0);
	lcd.write(1);
	lcd.setCursor(6,0);
	lcd.write(2);
	lcd.setCursor(7,0);
	lcd.write(3);
	lcd.setCursor(8,0);
	lcd.write(4);
	lcd.setCursor(9,0);
	lcd.write(5);
	lcd.setCursor(10,0);
	lcd.write(6);
	lcd.setCursor(11,0);
	lcd.write(7);
        delay(x);
        lcd.clear();
        lowerpart();
        lcd.setCursor(4,1);
	lcd.write(0);
	lcd.setCursor(5,1);
	lcd.write(1);
	lcd.setCursor(6,1);
	lcd.write(2);
	lcd.setCursor(7,1);
	lcd.write(3);
	lcd.setCursor(8,1);
	lcd.write(4);
	lcd.setCursor(9,1);
	lcd.write(5);
	lcd.setCursor(10,1);
	lcd.write(6);
	lcd.setCursor(11,1);
	lcd.write(7);
        delay(x);
        lcd.clear();
}	
	
void upperpart(){
        byte logoARD1a[8] = {B00000,B00000,B00001,B00011,B00111,B00111,B01110,B01110};
        byte logoARD2a[8] = {B00111,B11111,B11111,B11000,B10000,B00000,B00000,B00111};
        byte logoARD3a[8] = {B11100,B11111,B11111,B00011,B00000,B00000,B00000,B11100};
        byte logoARD4a[8] = {B00000,B00000,B11000,B11100,B11110,B01111,B01111,B00111};
        byte logoARD5a[8] = {B00000,B00000,B00011,B00111,B01111,B11110,B11110,B11100};
        byte logoARD6a[8] = {B00111,B11111,B11111,B11000,B00000,B00001,B00001,B00111};
        byte logoARD7a[8] = {B11100,B11111,B11111,B00011,B00001,B10000,B10000,B11100};
        byte logoARD8a[8] = {B00000,B00000,B10000,B11000,B11100,B11100,B01110,B01110};
	lcd.createChar(0, logoARD1a);
	lcd.createChar(1, logoARD2a);
	lcd.createChar(2, logoARD3a);
	lcd.createChar(3, logoARD4a);
	lcd.createChar(4, logoARD5a);
	lcd.createChar(5, logoARD6a);
	lcd.createChar(6, logoARD7a);
	lcd.createChar(7, logoARD8a);
}

void lowerpart(){
        byte logoARD9a[8] = {B01110,B01110,B00111,B00111,B00011,B00001,B00000,B00000};
        byte logoARD10a[8] = {B00111,B00000,B00000,B10000,B11000,B11111,B11111,B00111};
        byte logoARD11a[8] = {B11100,B00000,B00000,B00000,B00011,B11111,B11111,B11100};
        byte logoARD12a[8] = {B00111,B01111,B01111,B11110,B11100,B11000,B00000,B00000};
        byte logoARD13a[8] = {B11100,B11110,B11110,B01111,B00111,B00011,B00000,B00000};
        byte logoARD14a[8] = {B00111,B00001,B00001,B00000,B11000,B11111,B11111,B00111};
        byte logoARD15a[8] = {B11100,B10000,B10000,B00000,B00011,B11111,B11111,B11100};
        byte logoARD16a[8] = {B01110,B01110,B11100,B11100,B11000,B10000,B00000,B00000};
	lcd.createChar(0, logoARD9a);
	lcd.createChar(1, logoARD10a);
	lcd.createChar(2, logoARD11a);
	lcd.createChar(3, logoARD12a);
	lcd.createChar(4, logoARD13a);
	lcd.createChar(5, logoARD14a);
	lcd.createChar(6, logoARD15a);
	lcd.createChar(7, logoARD16a);
}
