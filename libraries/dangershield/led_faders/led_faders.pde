// Danger Board v1.0 Simple LED Faders
// By: Zach Hoeken (hoeken@gmail.com)
// More info: make.nycresistor.com/ds-1.0

//this is our setup stuff.
#include "_init.h"

void setup()
{
  ds_init();
}

//values for our sliders
int slider1_value = 0;
int slider2_value = 0;
int slider3_value = 0;

void loop()
{
  //read all our slider values.
  slider1_value = analogRead(SLIDER1_PIN) / 4;
  slider2_value = analogRead(SLIDER2_PIN) / 4;
  slider3_value = analogRead(SLIDER3_PIN) / 4;

  //write them directly to our leds.
  analogWrite(SLIDER1_LED_PIN, slider1_value);
  analogWrite(SLIDER2_LED_PIN, slider2_value);
  analogWrite(SLIDER3_LED_PIN, slider3_value);
}
