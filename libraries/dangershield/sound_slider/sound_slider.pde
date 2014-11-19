// Danger Board Gamma Corrected LED
// By: Zach Hoeken (hoeken@gmail.com)
// More info: make.nycresistor.com/ds-1.0

#include <math.h>
#include "_init.h"
#include "_data.h"

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

int seg_value = 0;

long idle = 0;
byte idle_num = 0;

void loop()
{
  //read our sliders
  raw1 = analogRead(SLIDER1_PIN);
  raw2 = analogRead(SLIDER2_PIN);
  raw3 = analogRead(SLIDER3_PIN);

  //correct for percieved brightness
  //pwm1 = gamma_correct(raw1, 3.0);
  //pwm2 = gamma_correct(raw2, 3.0);
  //pwm3 = gamma_correct(raw3, 3.0);
  pwm1 = 255;
  pwm2 = 255;
  pwm3 = 255;

  //make our leds run!
  if (digitalRead(BUTTON1_PIN))
  { 
    analogWrite(SLIDER1_LED_PIN, pwm1);
    seg_value = map(raw1, 0, 1023, 0, 9);

    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(1500 + analogRead(SLIDER1_PIN));
    digitalWrite(BUZZER_PIN, LOW);
    
    idle = 0;
  }
  else
    analogWrite(SLIDER1_LED_PIN, 0);
  
  //make our leds run!
  if (digitalRead(BUTTON2_PIN))
  { 
    analogWrite(SLIDER2_LED_PIN, pwm2);
    seg_value = map(raw2, 0, 1023, 0, 9);

    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(1000 + analogRead(SLIDER2_PIN));
    digitalWrite(BUZZER_PIN, LOW);
    
    idle = 0;
  }
  else
    analogWrite(SLIDER2_LED_PIN, 0);
    
  //make our leds run!
  if (digitalRead(BUTTON3_PIN))
  { 
    analogWrite(SLIDER3_LED_PIN, pwm3);
    seg_value = map(raw3, 0, 1023, 0, 9);

    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(500 + analogRead(SLIDER3_PIN));
    digitalWrite(BUZZER_PIN, LOW);
    
    idle = 0;
  }
  else
    analogWrite(SLIDER3_LED_PIN, 0);
   
 seg_value = constrain(seg_value, 0, 9);
  byte j = chardata[seg_value];
    
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, j);
  digitalWrite(LATCH_PIN, HIGH);
  
  //our idle program.
  idle++;
  if (idle > 10000)
  {
     byte j = chardata[idle_num];
    
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, j);
    digitalWrite(LATCH_PIN, HIGH);
  
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, LOW);
    delay(250);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED1_PIN, LOW);
    delay(250);
    
    idle = 10001;
    idle_num++;
    
    if (idle_num > 9)
      idle_num = 0;
  }
}

byte gamma_correct(byte pwm, double gamma)
{
  return (byte)(255.0 * pow(pwm/255.0, gamma));
}
