#include "WProgram.h"
byte gamma_correct(byte pwm, double gamma);
#include "math.h"
#include "_init.h"
// Danger Board Gamma Corrected LED
// By: Zach Hoeken (hoeken@gmail.com)
// More info: make.nycresistor.com/ds-1.0

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
  //read our sliders
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
  
  if (digitalRead(BUTTON1_PIN))
  {
    analogWrite(SLIDER1_LED_PIN, pwm1);
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(1000 + analogRead(SLIDER1_PIN));
    digitalWrite(BUZZER_PIN, LOW);
  }
  else
    analogWrite(SLIDER1_LED_PIN, 0);

  
  if (digitalRead(BUTTON2_PIN))
  {
    analogWrite(SLIDER2_LED_PIN, pwm2);
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(500 + analogRead(SLIDER2_PIN)/2);
    digitalWrite(BUZZER_PIN, LOW);
  }
  else
    analogWrite(SLIDER2_LED_PIN, 0);

  if (digitalRead(BUTTON3_PIN))
  {
    analogWrite(SLIDER3_LED_PIN, pwm3);
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(500 + analogRead(SLIDER3_PIN)/4);
    digitalWrite(BUZZER_PIN, LOW);
  }
  else
    analogWrite(SLIDER3_LED_PIN, 0);

}

byte gamma_correct(byte pwm, double gamma)
{
  return (byte)(255.0 * pow(pwm/255.0, gamma));
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

