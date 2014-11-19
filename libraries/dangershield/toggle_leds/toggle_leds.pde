// Danger Board v1.0 Toggle LEDS
// By: Zach Hoeken (hoeken@gmail.com)
// More info: make.nycresistor.com/ds-1.0

//our setup stuff
#include "_init.h"

void setup()
{
  ds_init();
}

//our button state
boolean button1_state = false;
boolean button2_state = false;
boolean button3_state = false;

//previous button state
boolean last_button1_state = false;
boolean last_button2_state = false;
boolean last_button3_state = false;

//our led states
boolean led1_state = true;
boolean led2_state = true;
boolean led3_state = true;

void loop()
{
  //read in the current value
  button1_state = digitalRead(BUTTON1_PIN);
  button2_state = digitalRead(BUTTON2_PIN);
  button3_state = digitalRead(BUTTON3_PIN);
  
  //we're looking for a low to high button press
  //if we got one, toggle the led
  if (button1_state && !last_button1_state)
    led1_state = !led1_state;
  if (button2_state && !last_button2_state)
    led2_state = !led2_state;
  if (button3_state && !last_button3_state)
    led3_state = !led3_state;

  //write our LED state value to the LED
  digitalWrite(SLIDER1_LED_PIN, led1_state);
  digitalWrite(SLIDER2_LED_PIN, led2_state);
  digitalWrite(SLIDER3_LED_PIN, led3_state);

  //save our current state as the last known state
  last_button1_state = button1_state;
  last_button2_state = button2_state;
  last_button3_state = button3_state;
}
