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

/* ijduino */
//#define	MAX_DEVICES	1
//#define	CLK_PIN		5  // or SCK
//#define	CS_PIN		21  // or SS
//#define	DATA_PIN	20  // or MOSI

/* Arduino Nano */
#define	MAX_DEVICES	1
#define	CLK_PIN		10  // or SCK
#define	CS_PIN		11  // or SS
#define	DATA_PIN	12  // or MOSI

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
int buttonPin = 13;          /* choose the input pin for the pushbutton */
int lastButtonState = LOW;   /* the previous reading from the input pin */
int lastDebounceTime = 0;    /* the last time the output pin was toggled */
int debounceDelay = 50;      /* the debounce time; increase if the output flickers */
int animation = 0;           /* Set animationcount to 0 */
int buttonState = LOW;       /* Set buttonstate to low */
int brightness = 5;          /* Set brightneslevel to 5 (0 to 15 are valid) */
const int animations = 28;   /* the number of animations we have */

int (*ijnimations[animations])() = {
ijhacklogo,
one,
two,
three,
four,
five,
six,
seven,
eight,
nine,
ten,
eleven,
twelve,
thirteen,
fourteen,
fifteen,
sixteen,
seventeen,
eighteen,
nineteen,
twenty,
twentyone,
twentytwo,
twentythree,
twentyfour,
twentyfive,
twentysix,
twentyseven
//twentyeight,
//twentynine,
//thirty


};

void setup() {
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
}

void loop() {
  (ijnimations)[animation]();
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
          return true;
        }
      }
    }

    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
    lastButtonState = reading;
  }
  return false;  /* don't skip the rest */
}

