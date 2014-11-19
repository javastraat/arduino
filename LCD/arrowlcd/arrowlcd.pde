/*  For the animation of the arrow we use 4 frames, of which only 3 frames are different.
    We are using frame 2 two times. In *arrow[4] we set up the frame order.
    for(char i=0; i<4; i++) means, do this 4 times and each time "i" will be 1 more.
    So the first time i=0 and arrows[i] will be arrowA1.
    The secend time i=1 and arrows[i] will be arrowA2 and so on till i=3 (i<4)
    Rotating Arrow by JO3RI 
    http://www.JO3RI.be/arduino
*/
#include  <LiquidCrystal.h>

LiquidCrystal lcd(8, 7, 6, 5, 4, 3, 2);

void Rarrow() {
  for(char i=0; i<4; i++) // go trough this part 4 times.
  {
    byte arrowA1[8] = { // set up arrow frame 1, look at the 0's and the 1's
      B00000, // ░░░░░
      B11000, // ██░░░
      B10100, // █░█░░
      B10010, // █░░█░
      B10010, // █░░█░
      B10100, // █░█░░
      B11000, // ██░░░
      B00000};// ░░░░░
    byte arrowA2[8] = { // set up arrow frame 2, look at the 0's and the 1's
      B00000, // ░░░░░
      B00000, // ░░░░░
      B11000, // ██░░░
      B10110, // █░██░
      B10110, // █░██░
      B11000, // ██░░░
      B00000, // ░░░░░
      B00000};// ░░░░░
    byte arrowA3[8] = { // set up arrow frame 3, look at the 0's and the 1's
      B00000, // ░░░░░
      B00000, // ░░░░░
      B00000, // ░░░░░
      B11110, // ████░
      B11110, // ████░
      B00000, // ░░░░░
      B00000, // ░░░░░
      B00000};// ░░░░░
    byte *arrows[4]={arrowA1,arrowA2,arrowA3,arrowA2}; // set up each animation step: frame 1, frame 2, frame 3, frame 2
    lcd.createChar(0, arrows[i]); //here we actually create the character on position zero
    lcd.setCursor(0,0); // here we set the cursor on location x and y (row and column)
    lcd.write(0); // write the character we defined in lcd.createChar to the LCD
    delay(250); // wait for 250 millisecs
  }
}

void setup() {
  lcd.begin(16, 2);
}

void loop(){
  Rarrow();
}


