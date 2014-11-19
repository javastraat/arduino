/*
||
|| @description
|| | This is a simple menu system, that uses input from the serial to change and use menuitems.
|| #
||
*/
/*                            IJduino 1.0
Requires:
 * 8X8 LED grid MAX7219
 * Mini Breadboard
 * Atmega328
 * Button
 * 2 Wires
 * 4.5 Volt
 (c) IJhack 2014 GNU GPL
http://ijhack.org/
 */
#include "LedControl.h"

#include <SubMenuItem.h>
#include <SubMenu.h>
#include <MenuItem.h>
#include <Menu.h>



/* ijduino */
//#define	MAX_DEVICES	1
//#define	CLK_PIN		5  // or SCK
//#define	CS_PIN		21  // or SS
//#define	DATA_PIN	20  // or MOSI

/* Arduino Nano */
#define	MAX_DEVICES	1
#define	CLK_PIN		11  // or SCK
#define	CS_PIN		12  // or SS
#define	DATA_PIN	13  // or MOSI

/*Arduino Mini */
//#define	MAX_DEVICES	1
//#define	CLK_PIN		7  // or SCK
//#define	CS_PIN		8  // or SS
//#define	DATA_PIN	9  // or MOSI


/* using VCC, GND, DIN 21, CS 21, CLK 5 for the MAX7219 */
// Create a new LedControl for 1 device...
LedControl lc = LedControl(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
/* lowPin used for ijduino*/
//int lowPin = 11;           /* ground pin for the buton ;-) */
int buttonPin = 8;          /* choose the input pin for the pushbutton */

int vala = 0;     // variable to store the read value
int valb = 0;     // variable to store the read value
int valc = 0;     // variable to store the read value
int lastButtonState = LOW;   /* the previous reading from the input pin */
int lastDebounceTime = 0;    /* the last time the output pin was toggled */
int debounceDelay = 50;      /* the debounce time; increase if the output flickers */
int animation = 0;           /* Set animationcount to 0 */
int buttonState = LOW;       /* Set buttonstate to low */
int brightness = 3;          /* Set brightneslevel to 5 (0 to 15 are valid) */
const int animations = 18;   /* the number of animations we have */


//initialize menu
Menu menu = Menu(menuUsed, menuChanged);
//initialize menuitems
MenuItem menuItem1 = MenuItem();
MenuItem menuItem2 = MenuItem();
MenuItem menuItem3 = MenuItem();
/*MenuItem menuItemWithSubmenu = MenuItem();
SubMenu subMenu = SubMenu(menuChanged);
SubMenuItem subMenuItem1 = SubMenuItem();
SubMenuItem subMenuItem2 = SubMenuItem();
SubMenuItem subMenuItem3 = SubMenuItem();
*/
//int (*ijnimations[animations])() = {
int (*ijnimations[animations])() = {
  ijhacklogo,
  //ghost,
  //invader,
  heart,
  invadesquid,
  jumper,
  ijlogowink,
  heartbeat,
  eq,
  invaderagain,
  pong,
  //snake,
  arrows,
  directions,
  //packman,
  diagonals,
  waterdrip,
  //aliens,
  blockanim,
  pulse,
  tewdoodles,
  xmastree,
  battery

};

void setup() {
  Serial.begin(9600);



  /*
      The MAX72XX is in power-saving mode on startup,
      we have to do a wakeup call
    */
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  /*
  * Set the brightness of the display.
  * Params:
  * int addr      the address of the display to control
  * int intensity the brightness of the display.
  *               Only values between 0(darkest) and 15(brightest) are valid.
  */
  lc.setIntensity(0, brightness);
  /*
  * Switch all Leds on the display off.
  * Params:
  * int addr The address of the display to control
  */
  lc.clearDisplay(0);
  /* setup pins for ijduino */
  //pinMode(buttonPin, INPUT_PULLUP);
  //pinMode(lowPin, OUTPUT);
  //digitalWrite(lowPin, LOW);

  //configure menu
  menu.addMenuItem(menuItem1);
  menu.addMenuItem(menuItem2);
  menu.addMenuItem(menuItem3);
  /*  menu.addMenuItem(menuItemWithSubmenu);
    menuItemWithSubmenu.addSubMenu(subMenu);
    subMenu.addSubMenuItem(subMenuItem1);
    subMenu.addSubMenuItem(subMenuItem2);
    subMenu.addSubMenuItem(subMenuItem3);
  */
//  menu.select(0);
  //  subMenu.select(0);
}
void loop() {
  // (ijnimations)[0]();
  //  (ijnimations)[animation]();
/*  vala = digitalRead(PIN_BUTTON_A);   // read the input pin
if (vala =1){menu.select(0);}
  valb = digitalRead(PIN_BUTTON_B);   // read the input pin
if (valb =1){menu.select(1);}
  valc = digitalRead(PIN_BUTTON_C);   // read the input pin
if (valc =1){menu.select(2);}
else;
*/

//vala = digitalRead(PIN_BUTTON_A);   // read the input pin
//if (digitalRead(2) == HIGH) {menu.select(1);}



// wait for the input to go HIGH
while(digitalRead(2) != HIGH)
{
menu.select(0);    // do nothing
// (ijnimations)[animation]();
 }
// wait for the input to go LOW
while(digitalRead(2) != LOW)

{
    // do nothing
}


//  val = digitalRead(inPin);   // read the input pin
 // digitalWrite(ledPin, val);    // sets the LED to the button's value
//break;
  /*if (Serial.available()) {
    switch (Serial.read()) {
      case '0':
        menu.select(0);
        break;
      case '1':
        menu.select(1);
        break;
      case '2':
        menu.select(2);
        break;
      case 'e':
        menu.use();
        break;
        /*
              case 'd':
                menu.down();
                break;
              case 'u':
                menu.up();
                break;
              case 'e':
                if (menu.isCurrentSubMenu()) {
                  subMenu.down(); //set index closer to 0
                }
                break;
              case 'b':
                if (menu.isCurrentSubMenu()) {
                  subMenu.up(); //set index closer to maximum
                }
                break;
        */
    }
//  }
//}

void menuChanged(ItemChangeEvent event) {
  Serial.println("menuChanged");
  if ( event == &menuItem1 ) {
    Serial.println("\tmenuItem1 selected"); //user feedback
    (ijnimations)[0]();
  }
  else if ( event == &menuItem2 ) {
    Serial.println("\tmenuItem2 selected"); //user feedback
    (ijnimations)[1]();
  }
  else if ( event == &menuItem3 ) {
    Serial.println("\tmenuItem3 selected"); //user feedback
    (ijnimations)[2]();
  }
  /*  else if ( event == &menuItemWithSubmenu ) {
      Serial.println("\tmenuItemWithSubmenu selected"); //user feedback
    }
    else if ( event == &subMenuItem1 ) {
      Serial.println("\tsubMenuItem1 selected"); //user feedback
    }
    else if ( event == &subMenuItem2 ) {
      Serial.println("\tsubMenuItem2 selected"); //user feedback
    }
    else if ( event == &subMenuItem3 ) {
      Serial.println("\tsubMenuItem3 selected"); //user feedback
    }
  */
}

void menuUsed(ItemUseEvent event) {
  Serial.println("menuUsed");
  if ( event == &menuItem1 ) {
    Serial.println("\tmenuItem1 used"); //user feedback
  }
  else if ( event == &menuItem2 ) {
    Serial.println("\tmenuItem2 used"); //user feedback
  }
  else if ( event == &menuItem3 ) {
    Serial.println("\tmenuItem3 used"); //user feedback
  }
  /*  else if ( event == &subMenuItem1 ) {
      Serial.println("\tsubMenuItem1 used"); //user feedback
    }
    else if ( event == &subMenuItem2 ) {
      Serial.println("\tsubMenuItem2 used"); //user feedback
    }
    else if ( event == &subMenuItem3 ) {
      Serial.println("\tsubMenuItem3 used"); //user feedback
    }
  */
}

bool render(byte* frame, int delaytime) {
  lc.setColumn(0, 0, frame[7]);
  lc.setColumn(0, 1, frame[6]);
  lc.setColumn(0, 2, frame[5]);
  lc.setColumn(0, 3, frame[4]);
  lc.setColumn(0, 4, frame[3]);
  lc.setColumn(0, 5, frame[2]);
  lc.setColumn(0, 6, frame[1]);
  lc.setColumn(0, 7, frame[0]);
  long startTime = millis();
  while ((startTime + delaytime) > millis()) {
  }

  return false;
}
void buttonread(int delaytime) {
  long startTime = millis();
  while ((startTime + delaytime) > millis()) {

    // read the state of the switch into a local variable:
    int reading = digitalRead(buttonPin);

    // check to see if you just pressed the button
    // (i.e. the input went from HIGH to LOW),  and you've waited
    // long enough since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState) {
      // reset the debouncing timer
      lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
      // whatever the reading is at, it's been there for longer
      // than the debounce delay, so take it as the actual current state:

      // if the button state has changed:
      if (reading != buttonState) {
        buttonState = reading;

        // only toggle the animation if the new button state is HIGH
        if (buttonState == LOW) {
          animation++;
          if (animation > animations) {
            animation = 0;
          }
          //          return true;
        }
      }
    }

    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
    lastButtonState = reading;
  }
  //  return false;  /* don't skip the rest */
}

