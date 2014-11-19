// Danger Board Gamma Corrected LED
// By: Zach Hoeken (hoeken@gmail.com)
// More info: make.nycresistor.com/ds-1.0

#include <math.h>
/*************************************************************************************************
* This file contains code and definitions to make your life using the Danger Shield really easy.
**************************************************************************************************/

//these are our digital input pins
#define BUTTON1_PIN 2
#define BUTTON2_PIN 7
#define BUTTON3_PIN 4

//these are our analog input pins
#define SLIDER1_PIN 2
#define SLIDER2_PIN 1
#define SLIDER3_PIN 0
#define LIGHT_SENSOR_PIN 3
#define TEMP_SENSOR_PIN 4
#define KNOCK_SENSOR_PIN 5

//these are our output pins
#define LED1_PIN 6
#define LED2_PIN 5
#define BUZZER_PIN 3
#define SLIDER1_LED_PIN 11
#define SLIDER2_LED_PIN 10
#define SLIDER3_LED_PIN 9

//these are for the shift register
#define LATCH_PIN 8
#define CLOCK_PIN 12
#define DATA_PIN 13

//our init and bootup function
void ds_init()
{
  //buttons are inputs.
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  pinMode(BUTTON3_PIN, INPUT);
  
  //leds and buzzers are outputs.
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SLIDER1_LED_PIN, OUTPUT);
  pinMode(SLIDER2_LED_PIN, OUTPUT);
  pinMode(SLIDER3_LED_PIN, OUTPUT);

  //shift register pins are output.
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  
  //flash our LEDs
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);
  digitalWrite(SLIDER1_LED_PIN, HIGH);
  digitalWrite(SLIDER2_LED_PIN, HIGH);
  digitalWrite(SLIDER3_LED_PIN, HIGH);

  //turn 7-segment on
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 255);
  digitalWrite(LATCH_PIN, HIGH);

  //buzz for a bit.
  for (int i=0; i<500; i++)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(1915);
    digitalWrite(BUZZER_PIN, LOW);
  }

  //turn off LEDs
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(SLIDER1_LED_PIN, LOW);
  digitalWrite(SLIDER2_LED_PIN, LOW);
  digitalWrite(SLIDER3_LED_PIN, LOW);

  //turn 7-segment off
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0);
  digitalWrite(LATCH_PIN, HIGH);

  //startup serial
  Serial.begin(9600);
  Serial.println("started");
}

void setup()
{
  ds_init();
}

int raw1 = 0;
int raw2 = 0;
int raw3 = 0;

int pwm1 = 0;
int pwm2 = 0;
int pwm3 = 0;
int pwm4 = 0;
int pwm5 = 0;

void loop()
{
  //read our sliders (convert them to 0-255 range too)
  raw1 = analogRead(SLIDER1_PIN) / 4;
  raw2 = analogRead(SLIDER2_PIN) / 4;
  raw3 = analogRead(SLIDER3_PIN) / 4;

  //correct for percieved brightness
  pwm1 = gamma_correct(raw1, 3.0);
  pwm2 = gamma_correct(raw2, 3.0);
  pwm3 = gamma_correct(raw3, 3.0);
  pwm4 = gamma_correct(raw3, 3.0);
  pwm5 = gamma_correct(raw3, 3.0);

  //make our leds run!
  analogWrite(SLIDER1_LED_PIN, pwm1);
  analogWrite(SLIDER2_LED_PIN, pwm2);
  analogWrite(SLIDER3_LED_PIN, pwm3);
  analogWrite(LED1_PIN, pwm4);
  analogWrite(LED2_PIN, pwm5);
}

byte gamma_correct(byte pwm, double gamma)
{
  return (byte)(255.0 * pow(pwm/255.0, gamma));
}
