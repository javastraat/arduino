// Danger Board v1.0 Button Lightup Script
// By: Zach Hoeken (hoeken@gmail.com)
// More info: make.nycresistor.com/ds-1.0

#include "_init.h"

void setup()
{
  ds_init();
}

//our button state variables
boolean button1State = false;
boolean button2State = false;
boolean button3State = false;

void loop()
{
  //read all our buttons
  button1State = digitalRead(BUTTON1_PIN);
  button2State = digitalRead(BUTTON2_PIN);
  button3State = digitalRead(BUTTON3_PIN);

  //write all our LEDS
  digitalWrite(SLIDER1_LED_PIN, button1State);
  digitalWrite(SLIDER2_LED_PIN, button2State);
  digitalWrite(SLIDER3_LED_PIN, button3State);
}
