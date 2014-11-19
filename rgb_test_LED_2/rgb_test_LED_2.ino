/*
 Some Sample code of how to use your IR remote

 * Lets get started:

 The IR sensor's pins are attached to Arduino as so:
 Pin 1 to Vout (pin 11 on Arduino)
 Pin 2 to GND
 Pin 3 to Vcc (+5v from Arduino)

*/

#include <IRremote.h>
#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 3
#define FADESPEED 1500 // make this higher to slow down
long randNumberi;
long randNumberii;
long randNumberiii;
int bright = 5;
//int val_blue=0;
//int val_green=0;
//int val_red=0;

int IRpin = 8;  // pin for the IR sensor
int LED = 9;    // LED pin
IRrecv irrecv(IRpin);
decode_results results;

boolean LEDon = true; // initializing LEDon as true

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(LED, OUTPUT);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  randomSeed(analogRead(0));
}

void loop()
{

  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX);
    Serial.println(results.value, DEC);

    irrecv.resume();   // Receive the next value
  }

  switch (results.value)
  {


    case 16195807:
      // feed my dog for me
      Serial.println("Red");
      //int val_red=(25.5*bright);
      analogWrite (REDPIN, 255);
      analogWrite (GREENPIN, 0);
      analogWrite (BLUEPIN, 0);
      break;

    case 16191727:
      // feed my dog for me
      Serial.println("Dark Orange");
      analogWrite (REDPIN, 255);
      analogWrite (GREENPIN, 140);
      analogWrite (BLUEPIN, 0);
      break;

    case 16199887:
      // feed my dog for me
      Serial.println("Orange Red");
      analogWrite (REDPIN, 255);
      analogWrite (GREENPIN, 69);
      analogWrite (BLUEPIN, 0);
      break;

    case 16189687:
      // feed my dog for me
      Serial.println("Orange");
      analogWrite (REDPIN, 255);
      analogWrite (GREENPIN, 165);
      analogWrite (BLUEPIN, 0);
      break;

    case 16197847:
      // feed my dog for me
      Serial.println("Yellow");
      analogWrite (REDPIN, 255);
      analogWrite (GREENPIN, 255);
      analogWrite (BLUEPIN, 0);
      break;

    case 16228447:
      // feed my dog for me
      Serial.println("Green");
      analogWrite (REDPIN, 0);
      analogWrite (GREENPIN, 128);
      analogWrite (BLUEPIN, 0);
      break;

    case 16224367:
      // feed my dog for me
      Serial.println("Lime");
      analogWrite (REDPIN, 0);
      analogWrite (GREENPIN, 255);
      analogWrite (BLUEPIN, 0);
      break;

    case 16232527:
      // feed my dog for me
      Serial.println("Cyan");
      analogWrite (REDPIN, 0);
      analogWrite (GREENPIN, 255);
      analogWrite (BLUEPIN, 255);
      break;

    case 16222327:
      // feed my dog for me
      Serial.println("Royal Blue");
      analogWrite (REDPIN, 65);
      analogWrite (GREENPIN, 105);
      analogWrite (BLUEPIN, 225);
      break;

    case 16230487:
      // feed my dog for me
      Serial.println("Blueish");
      analogWrite (REDPIN, 255);
      analogWrite (GREENPIN, 0);
      analogWrite (BLUEPIN, 0);
      break;

    case 16212127:
      // feed my dog for me
      Serial.println("Blue");
      analogWrite (REDPIN, 0);
      analogWrite (GREENPIN, 0);
      analogWrite (BLUEPIN, 255);
      break;

    case 16208047:
      // feed my dog for me
      Serial.println("Dark Blue");
      analogWrite (REDPIN, 255);
      analogWrite (GREENPIN, 0);
      analogWrite (BLUEPIN, 0);
      break;

    case 16216207:
      // feed my dog for me
      Serial.println("Indigio");
      analogWrite (REDPIN, 75);
      analogWrite (GREENPIN, 0);
      analogWrite (BLUEPIN, 130);
      break;

    case 16206007:
      // feed my dog for me
      Serial.println("Blue Violet");
      analogWrite (REDPIN, 138);
      analogWrite (GREENPIN, 43);
      analogWrite (BLUEPIN, 226);
      break;

    case 16214167:
      // feed my dog for me
      Serial.println("Magenta");
      analogWrite (REDPIN, 255);
      analogWrite (GREENPIN, 0);
      analogWrite (BLUEPIN, 255);
      break;

    case 16244767:
      // feed my dog for me
      Serial.println("White");
      analogWrite (REDPIN, 20);
      analogWrite (GREENPIN, 20);
      analogWrite (BLUEPIN, 20);
      break;

    case 16240687:
      // feed my dog for me
      Serial.println("Flash");
      analogWrite (REDPIN, 255);
      analogWrite (GREENPIN, 0);
      analogWrite (BLUEPIN, 0);
      delay(500);
      analogWrite (REDPIN, 0);
      analogWrite (GREENPIN, 255);
      analogWrite (BLUEPIN, 0);
      delay(500);
      analogWrite (REDPIN, 0);
      analogWrite (GREENPIN, 0);
      analogWrite (BLUEPIN, 0);
      delay(500);
      break;

    case 16248847:
      // feed my dog for me
      Serial.println("Strobe");
      analogWrite (REDPIN, 255);
      analogWrite (GREENPIN, 0);
      analogWrite (BLUEPIN, 0);
      delay(100);
      analogWrite (REDPIN, 0);
      analogWrite (GREENPIN, 255);
      analogWrite (BLUEPIN, 0);
      delay(100);
      analogWrite (REDPIN, 0);
      analogWrite (GREENPIN, 0);
      analogWrite (BLUEPIN, 0);
      delay(100);
      break;

    case 16238647:
      // feed my dog for me
      Serial.println("Fade");
      analogWrite (REDPIN, 255);
      analogWrite (GREENPIN, 0);
      analogWrite (BLUEPIN, 0);
      break;

    case 16246807:
      // feed my dog for me
      Serial.println("Smooth");
      analogWrite (REDPIN, 255);
      analogWrite (GREENPIN, 0);
      analogWrite (BLUEPIN, 0);
      break;

    case 16187647:
      // feed my dog for me
      Serial.println("Brightness +");
      Serial.println(bright);
      bright = bright + 1;
      if (bright > 10)
      {
        bright = 10;
      }
      Serial.println(bright);
      break;

    case 16220287:
      // feed my dog for me
      Serial.println("Brightness -");
      Serial.println(bright);
      bright = bright - 1;
      if (bright < 0)
      {
        bright = 1;
      }
      Serial.println(bright);
      break;

    case 16203967:
      // feed my dog for me
      Serial.println("OFF");
      analogWrite (REDPIN, 0);
      analogWrite (GREENPIN, 0);
      analogWrite (BLUEPIN, 0);
      break;

    case 16236607:
      // feed my dog for me
      Serial.println("ON");
      analogWrite (REDPIN, 10);
      analogWrite (GREENPIN, 10);
      analogWrite (BLUEPIN, 10);
      break;


    case 33489661:
      // feed my dog for me
      break;


    default:
      break;

  }

}
