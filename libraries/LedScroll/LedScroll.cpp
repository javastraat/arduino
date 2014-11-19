/*
  LedScroll.h - Library for flashing Morse code.
  Created by Remy Younes, April 7, 2013.
  Released into the public domain.
*/

#include "Arduino.h"
#include "LedScroll.h"
#include "LedControl.h"

LedScroll::LedScroll(LedControl *lc)
{
  //LedControl
	_lc  = lc;
	//Scroll Delay 
	setDelay(50);
	//size of current
	_bannerSize = 0;
}

//=============================
// PRINT & SCROLL ON LED MATRIX 
//=============================
void LedScroll::print(String message)
{
  //add padding(optional): helps with scroll rotation
  message = "  " + message + "  ";
  
  //size of the banner
  byte* banner = constructBanner(message);
  printBanner(banner);
  //deallocate, once we're done printing
  delete [] banner;
}

// SCROLL SPEED
void LedScroll::setDelay(long millis)
{
  _delaytime = millis;
}

//=======
//HELPERS
//=======

//construct a printable banner out of each message character
byte* LedScroll::constructBanner(String s)
{
  //calculate _bannerSize for memory alloc and loop
  _bannerSize =  s.length() * (CLEN + SPACING); 
  // banner is a side-by-side collection of the letters that spell out message
  // m allocation for banner
  byte* banner = new byte[_bannerSize];
  int index, offset;
  char c;

  //insert letters in banner
  for(int i=0; i<s.length(); i++){
    offset = (CLEN + SPACING) * i;
    c = tolower(s[i]);
    index = getCharIndex(c);
    // print each row of each char on banner
    for(int j=0; j<CLEN; j++){
      if(j<CLEN){
        banner[offset+j] = CHARACTERS[index][j];
        printByte(CHARACTERS[0][j]);
      }
    }
    // print spacing
    for(int k=0; k<SPACING; k++){
      banner[offset+CLEN+k] = B00000000; 
    }
  }
  return banner;
}

//DEBUG METHOD: byte to string 
void LedScroll::printByte(byte b)
{
  Serial.print("\nB");
  for(int i=0; i<8; i++){
    Serial.print(b % (int)pow(2,i) == 0 ? 0:1);
  }
}

//CONVERT CURRENT MESSAGE CHARACTER TO INDEX IN CHARACTER SET
int LedScroll::getCharIndex(char c)
{
  //index 26 represents a space (shown by default if unknown char)
  int index = 26;
  
  //if char is a-z, find index
  if ( 'a' <= c && c <= 'z') {
    index = c - 'a';
  }
  return index;
}

//ITERATE THROUGH BANNER AND PRINT EACH ROW
void LedScroll::printBanner(byte* banner){
  for( int i=0; i < _bannerSize-(LCD_WIDTH); i++){
    //Update rows from i to i+j (8 rows)
    for(int j=0; j<LCD_WIDTH; j++){
      (*_lc).setRow(0, j, banner[i+j]); 
    }
    //scroll delay
    delay(_delaytime);
  }
}