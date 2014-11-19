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
#include <JoystickShield.h> // include JoystickShield Library

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

JoystickShield joystickShield; // create an instance of JoystickShield object
//int buttonPin = 4;          /* choose the input pin for the pushbutton */
int lastDebounceTime = 0;    /* the last time the output pin was toggled */
int debounceDelay = 50;      /* the debounce time; increase if the output flickers */
int animation = 0;           /* Set animationcount to 0 */
int buttonState = LOW;       /* Set buttonstate to low */
int brightness = 3;          /* Set brightneslevel to 5 (0 to 15 are valid) */
const int animations = 17;   /* the number of animations we have */

int (*ijnimations[animations])() = {
  ijhacklogo,
  //ghost,
  //invader,
  heart,
  //invadesquid,
  jumper,
  ijlogowink,
  heartbeat,
  eq,
  //invaderagain,
  pong,
  snake,
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
  
    delay(100);
    // new calibration function
    joystickShield.calibrateJoystick();
    
    // predefined Joystick to Pins 0 and 1.
    // Change it if you are using a different shield
    // setJoystickPins(0, 1);
	
    // predefined buttons to the following pins.
    // change it if you are using a different shield.
    // setButtonPins(pinJoystickButton, pinUp, pinRight, pinDown, pinLeft, pinF, pinE);
    // to deactivate a button use a pin outside of the range of the arduino e.g. 255, but not above
    // setButtonPins(8, 2, 3, 4, 5, 7, 6);
    
    // Joystick Callbacks
    joystickShield.onJSUp(&up);
    joystickShield.onJSRightUp(&rightUp);
    joystickShield.onJSRight(&right);
    joystickShield.onJSRightDown(&rightDown);
    joystickShield.onJSDown(&down);
    joystickShield.onJSLeftDown(&leftDown);
    joystickShield.onJSLeft(&left);
    joystickShield.onJSLeftUp(&leftUp);
    joystickShield.onJSnotCenter(&notCenter);

    // Button Callbacks
    joystickShield.onJoystickButton(&joystickButton);
    joystickShield.onUpButton(&upButton);
    joystickShield.onRightButton(&rightButton);
    joystickShield.onDownButton(&downButton);
    joystickShield.onLeftButton(&leftButton);
    joystickShield.onFButton(&FButton);
    joystickShield.onEButton(&EButton);
}


void loop() {
    joystickShield.processCallbacks(); // you don't have do anything else
 // (ijnimations)[animation]();
  delay(100);
}

/** Define Callback Function **/
void up() {
    Serial.println("Up from callback");
}

void rightUp() {
    Serial.println("Right Up from callback");
}

void right() {
    Serial.println("Right from callback");
}

void rightDown() {
    Serial.println("Right Down from callback");
}

void down() {
    Serial.println("Down from callback");
}

void leftDown() {
    Serial.println("Left Down from callback");
}

void left() {
    Serial.println("Left from callback");
}

void leftUp() {
    Serial.println("Left Up from callback");
}

void joystickButton() {
    Serial.println("Joystick from callback");
}

void upButton() {
    Serial.println("Up Button from callback");
  (ijnimations)[1]();
}

void rightButton() {
    Serial.println("Right Button from callback");
  (ijnimations)[2]();
}

void downButton() {
    Serial.println("Down Button from callback");
  (ijnimations)[4]();
}

void leftButton() {
    Serial.println("Left Button from callback");
  (ijnimations)[5]();
}

void EButton() {
    Serial.println("E Button from callback");
  (ijnimations)[6]();
}
void FButton() {
    Serial.println("F Button from callback");
  (ijnimations)[animation]();
}


void notCenter() {
   Serial.println("Not Center from callback");
   // new position functions
   Serial.print("x ");	Serial.print(joystickShield.xAmplitude());Serial.print(" y ");Serial.println(joystickShield.yAmplitude());
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
  //  int reading = digitalRead(buttonPin);

    // check to see if you just pressed the button
    // (i.e. the input went from HIGH to LOW),  and you've waited
    // long enough since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    //if (reading != lastButtonState) {
      // reset the debouncing timer
      lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
      // whatever the reading is at, it's been there for longer
      // than the debounce delay, so take it as the actual current state:

      // if the button state has changed:
    //  if (reading != buttonState) {
      //  buttonState = reading;

        // only toggle the animation if the new button state is HIGH
        //if (buttonState == LOW) {
          animation++;
          if (animation > animations) {
            animation = 0;
          }
          return true;
        }
      }
 //   }

    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
 //   lastButtonState = reading;
//  }
//  return false;  /* don't skip the rest */
//}

