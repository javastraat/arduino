// Danger Board v1.0 Demo Script
// By: Zach Hoeken (hoeken@gmail.com)
// More info: make.nycresistor.com/ds-1.0

//our setup stuff
#include "_init.h"

void setup()
{
  ds_init();

  //light sensor is decent for random stuff.
  randomSeed(analogRead(LIGHT_SENSOR_PIN));
}

int slider1_value = 0;
int slider2_value = 0;
int slider3_value = 0;
int old_slider1_value = 0;
int old_slider2_value = 0;
int old_slider3_value = 0;
int slider_map = 0;

byte seg_value = 0;

int light_value = 0;
int knock_value = 0;

int raw_temp = 0;
int celsius = 0;
int fahrenheit = 0;

boolean button1_state = false;
boolean button2_state = false;
boolean button3_state = false;

int wait = 0;

byte char_data[] = {
  B00111111, //0
  B00000110, //1
  B01011011, //2
  B01001111, //3
  B01100110, //4
  B01101101, //5
  B01111100, //6
  B00000111, //7
  B01111111, //8
  B01100111 //9
};

void loop()
{
  //read our digital values
  button1_state = digitalRead(BUTTON1_PIN);
  button2_state = digitalRead(BUTTON2_PIN);
  button3_state = digitalRead(BUTTON3_PIN);

  //read all our analog values
  slider1_value = analogRead(SLIDER1_PIN);
  slider2_value = analogRead(SLIDER2_PIN);
  slider3_value = analogRead(SLIDER3_PIN);
  light_value = analogRead(LIGHT_SENSOR_PIN);
  knock_value = analogRead(KNOCK_SENSOR_PIN);
  raw_temp = analogRead(TEMP_SENSOR_PIN);

  //convert temp to real temps
  celsius = (5.0 * raw_temp * 100.0) / 1024.0;
  fahrenheit = (celsius * 1.8) + 32;

  //now, randomly set the led on 25% of the time.
  if (random(0, 101) <= 25 || button1_state)
    digitalWrite(SLIDER1_LED_PIN, HIGH);
  else
    digitalWrite(SLIDER1_LED_PIN, LOW);

  //now, randomly set the led on 50% of the time.
  if (random(0, 101) <= 50 || button2_state)
    digitalWrite(SLIDER2_LED_PIN, HIGH);
  else
    digitalWrite(SLIDER2_LED_PIN, LOW);

  //now, randomly set the led on 75% of the time.
  if (random(0, 101) <= 75 || button3_state)
    digitalWrite(SLIDER3_LED_PIN, HIGH);
  else
    digitalWrite(SLIDER3_LED_PIN, LOW);

  //now, randomly set the led on 15% of the time.
  if (random(0, 101) <= 15)
    digitalWrite(LED1_PIN, HIGH);
  else
    digitalWrite(LED1_PIN, LOW);
    
  //now, randomly set the led on 85% of the time.
  if (random(0, 101) <= 85)
    digitalWrite(LED2_PIN, HIGH);
  else
    digitalWrite(LED2_PIN, LOW);

  if (abs(slider1_value - old_slider1_value) > 10)
  {
    slider_map = map(slider1_value, 0, 1023, 0, 9);
    slider_map = constrain(slider_map, 0, 9);
    slider_map = 9 - slider_map;
    seg_value = char_data[slider_map];
    wait = -12;
  }
  else if (abs(slider2_value - old_slider2_value) > 10)
  {
    slider_map = map(slider2_value, 0, 1023, 0, 9);
    slider_map = constrain(slider_map, 0, 9);
    slider_map = 9 - slider_map;
    seg_value = char_data[slider_map];
    wait = -12;
  }
  else if (abs(slider3_value - old_slider3_value) > 10)
  {
    slider_map = map(slider3_value, 0, 1023, 0, 9);
    slider_map = constrain(slider_map, 0, 9);
    slider_map = 9 - slider_map;
    seg_value = char_data[slider_map];
    wait = -12;
  }
  else if (wait > 0)
  {
    seg_value = random(0, 255);
    wait = -2;
  }

  if (wait <= 0)
  {
   //show a random byte on the 7-segment
   digitalWrite(LATCH_PIN, LOW);
   shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, seg_value);
   digitalWrite(LATCH_PIN, HIGH);
  }
  
  delay(75);
  
  old_slider1_value = slider1_value;
  old_slider2_value = slider2_value;
  old_slider3_value = slider3_value;
  wait++;
}
