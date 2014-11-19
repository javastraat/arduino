#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 7, 6, 5, 4, 3, 2);

byte smiley1[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};
byte smiley2[8] = {
  B00000,
  B10001,
  B00100,
  B00000,
  B10001,
  B01110,
  B00000,
};
byte smiley3[8] = {
  B00000,
  B10001,
  B00100,
  B00100,
  B10001,
  B01110,
  B00000,
};
byte smiley4[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};
byte smiley5[8] = {
  B00000,
  B10001,
  B00100,
  B00000,
  B10001,
  B01110,
  B00000,
};
byte smiley6[8] = {
  B00000,
  B10001,
  B00100,
  B00100,
  B10001,
  B01110,
  B00000,
};
byte smiley7[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};
byte smiley8[8] = {
  B00000,
  B10001,
  B00100,
  B00000,
  B10001,
  B01110,
  B00000,
};



byte smiley11[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00001,
};
byte smiley12[8] = {
  B00000,
  B10001,
  B00100,
  B00000,
  B10001,
  B01110,
  B00001,
};
byte smiley13[8] = {
  B00000,
  B10001,
  B00100,
  B00100,
  B10001,
  B01110,
  B00001,
};
byte smiley14[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00001,
};
byte smiley15[8] = {
  B00000,
  B10001,
  B00100,
  B00000,
  B10001,
  B01110,
  B00001,
};
byte smiley16[8] = {
  B00000,
  B10001,
  B00100,
  B00100,
  B10001,
  B01110,
  B00001,
};
byte smiley17[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00001,
};
byte smiley18[8] = {
  B00000,
  B10001,
  B00100,
  B00000,
  B10001,
  B01110,
  B00001,
};



void setup() {
  
  //lcd.begin(16, 2);  
  
  lcd.createChar(0, smiley1);
  lcd.createChar(1, smiley2);
  lcd.createChar(2, smiley3);
  lcd.createChar(3, smiley4);
  lcd.createChar(4, smiley5);
  lcd.createChar(5, smiley6);
  lcd.createChar(6, smiley7);
  lcd.createChar(7, smiley8);
  
  lcd.begin(16, 2);
  
  lcd.write(0);
  lcd.write(1);
  lcd.write(2);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
  lcd.write(6);
  lcd.write(7);
  
 lcd.setCursor(0,1);
  lcd.createChar(0, smiley11);
  //lcd.createChar(1, smiley12);
  //lcd.createChar(2, smiley13);
  //lcd.createChar(3, smiley14);
  //lcd.createChar(4, smiley15);
  //lcd.createChar(5, smiley16);
  //lcd.createChar(6, smiley17);
  //lcd.createChar(7, smiley18);
  lcd.write(0);
  lcd.write(1);
  lcd.write(2);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
  lcd.write(6);
  lcd.write(7);
  
  
}

void loop() {}
