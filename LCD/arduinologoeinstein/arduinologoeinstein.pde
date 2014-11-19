#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 7, 6, 5, 4, 3, 2);

byte logoa1[8] = {
  B00000,
  B00000,
  B00001,
  B00011,
  B00111,
  B00111,
  B01110,
  B01110};
byte logoa2[8] = {
  B00111,
  B11111,
  B11111,
  B11000,
  B10000,
  B00000,
  B00000,
  B00111};
byte logoa3[8] = {
  B11100,
  B11111,
  B11111,
  B00011,
  B00000,
  B00000,
  B00000,
  B11100};
byte logoa4[8] = {
  B00000,
  B00000,
  B11000,
  B11111,
  B11111,
  B01111,
  B01111,
  B00111};
byte logoa5[8] = {
  B00000,
  B00000,
  B00011,
  B00111,
  B01111,
  B11110,
  B11110,
  B11100};
byte logoa6[8] = {
  B00111,
  B11111,
  B11111,
  B11000,
  B00000,
  B00001,
  B00001,
  B00111};
byte logoa7[8] = {
  B11100,
  B11111,
  B11111,
  B00011,
  B00001,
  B10000,
  B10000,
  B11100};
byte logoa8[8] = {
  B00000,
  B00000,
  B10000,
  B11000,
  B11100,
  B11100,
  B01110,
  B01110};

byte logob1[8] = {
  B01110,
  B01110,
  B00111,
  B00111,
  B00011,
  B00001,
  B00000,
  B00000};
byte logob2[8] = {
  B00111,
  B00000,
  B00000,
  B10000,
  B11000,
  B11111,
  B11111,
  B00111};
byte logob3[8] = {
  B11100,
  B00000,
  B00000,
  B00000,
  B00011,
  B11111,
  B11111,
  B11100};
byte logob4[8] = {
  B01110,
  B01110,
  B11100,
  B11100,
  B11000,
  B10000,
  B00000,
  B00000};
byte logob5[8] = {
  B11100,
  B11110,
  B11110,
  B01111,
  B00111,
  B00011,
  B00000,
  B00000};
byte logob6[8] = {
  B00111,
  B00001,
  B00001,
  B00000,
  B11000,
  B11111,
  B11111,
  B00111};
byte logob7[8] = {
  B11100,
  B10000,
  B10000,
  B00000,
  B00011,
  B11111,
  B11111,
  B11100};
byte logob8[8] = {
  B01110,
  B01110,
  B11100,
  B11100,
  B11000,
  B10000,
  B00000,
  B00000};


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);

  // lcd.setCursor(0, 0);
  // lcd.print("* Arduino Logo *");
  // lcd.setCursor(0, 1);
  // lcd.print("Booting.........");
  // delay (2000);

  // clear the LCD screen:
  lcd.clear();
  logo1();
  delay(1000);
  lcd.setCursor(4,1);
  logo2(); 
}

void loop() {
//logo1();
//logo2();
}

void logo1(){
  lcd.createChar(0, logob1);
  lcd.createChar(1, logoa1);
  lcd.createChar(2, logob2);
  lcd.createChar(3, logoa2);
  lcd.createChar(4, logob3);
  lcd.createChar(5, logoa3);
  lcd.createChar(6, logob4);
  lcd.createChar(7, logoa4);

  lcd.setCursor(0,1);
  lcd.write(0);
  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.setCursor(1,1);
  lcd.write(2);
  lcd.setCursor(1,0);
  lcd.write(3);
  lcd.setCursor(2,1);
  lcd.write(4);
  lcd.setCursor(2,0);
  lcd.write(5);
  lcd.setCursor(3,1);
  lcd.write(6);
  lcd.setCursor(3,0);
  lcd.write(7);
}

void logo2(){
  lcd.createChar(0, logob5);
  lcd.createChar(1, logoa5);
  lcd.createChar(2, logob6);
  lcd.createChar(3, logoa6);
  lcd.createChar(4, logob7);
  lcd.createChar(5, logoa7);
  lcd.createChar(6, logob8);
  lcd.createChar(7, logoa8);

  lcd.setCursor(4,1);
  lcd.write(0);
  lcd.setCursor(4,0);
  lcd.write(1);
  lcd.setCursor(5,1);
  lcd.write(2);
  lcd.setCursor(5,0);
  lcd.write(3);
  lcd.setCursor(6,1);
  lcd.write(4);
  lcd.setCursor(6,0);
  lcd.write(5);
  lcd.setCursor(7,1);
  lcd.write(6);
  lcd.print("Arduino");
  lcd.setCursor(7,0);
  lcd.write(7);
  lcd.print("booting");  
}























