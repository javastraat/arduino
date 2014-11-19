#include <LiquidCrystal.h>

#include <LiquidCrystal.h>

// LiquidCrystal display with:
// rs on pin 12
// rw on pin 11
// enable on pin 10
// d4, d5, d6, d7 on pins 5, 4, 3, 2
LiquidCrystal lcd(8, 7, 6, 5, 4, 3, 2);
int ledPin = 9;		    // LED connected to digital pin 13
int recvPin=0;
int wait=2500;
int wait1=500;
int val=0;
void setup()
{
  // Print a message to the LCD.

  lcd.begin(16,2);
}
void loop() {

  lcd.setCursor(0, 0);
  lcd.print("*Hello Einstein*");
  lcd.setCursor(0, 1);
  lcd.print("This is a test !");
  delay (wait);
  lcd.clear();
  delay (wait1);
}



