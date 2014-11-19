/*
A set of custom made large numbers for a 16x2 LCD using the
LiquidCrystal librabry. Works with displays compatible with the
Hitachi HD44780 driver.

The Cuicuit:
  LCD RS pin to D12
  LCD Enable pin to D11
  LCD D4 pin to D5
  LCD D5 pin to D4
  LCD D6 pin to D3
  LCD D7 pin to D2
  LCD Vee tied to a pot to control brightness
  LCD Vss and R/W tied to ground
  LCD Vcc to +5V
  LCD pin 15 tied to pushbutton for control of backlight
  
This code has the custum numbers counting from 0 to 9 on
the left hand side of the LCD with a 1 sec delay between
numbers. On the right is a counter tracking the number of
times the program has looped.
  
Made by Michael Pilcher
2/9/2010
*/

// include the library
#include <LiquidCrystal.h>

// initialize the interface pins
LiquidCrystal lcd(8, 7, 6, 5, 4, 3, 2);

// the 8 arrays that form each segment of the custom numbers
byte LT[8] =
{
  B00111,
  B01111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte UB[8] =
{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte RT[8] =
{
  B11100,
  B11110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte LL[8] =
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01111,
  B00111
};
byte LB[8] =
{
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
};
byte LR[8] =
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11110,
  B11100
};
byte MB[8] =
{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
};
byte block[8] =
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

// loop counter
int count = 0;
  
void setup()
{
  // assignes each segment a write number
  lcd.createChar(0,LT);
  lcd.createChar(1,UB);
  lcd.createChar(2,RT);
  lcd.createChar(3,LL);
  lcd.createChar(4,LB);
  lcd.createChar(5,LR);
  lcd.createChar(6,MB);
  lcd.createChar(7,block);
  
  // sets the LCD's rows and colums:
  lcd.begin(16, 2);
        
}

void custom0()
{ // uses segments to build the number 0
  lcd.setCursor(0,0); // set cursor to column 0, line 0 (first row)
  lcd.write(0);  // call each segment to create
  lcd.write(1);  // top half of the number
  lcd.write(2);
  lcd.setCursor(0, 1); // set cursor to colum 0, line 1 (second row)
  lcd.write(3);  // call each segment to create
  lcd.write(4);  // bottom half of the number
  lcd.write(5);
}

void custom1()
{
  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(0,1);
  lcd.write(4);
  lcd.write(7);
  lcd.write(4);
}

void custom2()
{
  lcd.setCursor(0,0);
  lcd.write(6);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(0, 1);
  lcd.write(3);
  lcd.write(4);
  lcd.write(4);
}

void custom3()
{
  lcd.setCursor(0,0);
  lcd.write(6);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(0, 1);
  lcd.write(4);
  lcd.write(4);
  lcd.write(5);
}

void custom4()
{
  lcd.setCursor(0,0);
  lcd.write(3);
  lcd.write(4);
  lcd.write(7);
  lcd.setCursor(2, 1);
  lcd.write(7);
}

void custom5()
{
  lcd.setCursor(0,0);
  lcd.write(3);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(0, 1);
  lcd.write(4);
  lcd.write(4);
  lcd.write(5);
}

void custom6()
{
  lcd.setCursor(0,0);
  lcd.write(0);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(0, 1);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
}

void custom7()
{
  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(2, 1);
  lcd.write(7);
}

void custom8()
{
  lcd.setCursor(0,0);
  lcd.write(0);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(0, 1);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
}

void custom9()
{
  lcd.setCursor(0,0);
  lcd.write(0);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(2, 1);
  lcd.write(7);
}

void clearnumber()
{ // clears the area the custom number is displayed in
 lcd.setCursor(0,0);
 lcd.print("   ");
 lcd.setCursor(0,1);
 lcd.print("   ");
}

void loop()
{
  lcd.setCursor(5,0);  // puts cursor at columb 5, line 0
  lcd.print("Loop Count:");  // displays the words on LCD
  lcd.setCursor(9,1);  // sets cursor to column 9, line 1
  lcd.print(count);  // displays loop count on LCD
  clearnumber();  // clears the custom number
  custom0();    // displays custom 0 on the LCD
  delay(1000);  // delay 1 second
  clearnumber();
  custom1();
  delay(1000);
  clearnumber();
  custom2();
  delay(1000);
  clearnumber();
  custom3();
  delay(1000);
  clearnumber();
  custom4();
  delay(1000);
  clearnumber();
  custom5();
  delay(1000);
  clearnumber();
  custom6();
  delay(1000);
  clearnumber();
  custom7();
  delay(1000);
  clearnumber();
  custom8();
  delay(1000);
  clearnumber();
  custom9();
  delay(1000);
  count++;  // adds 1 to the loop count
} 

