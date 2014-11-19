#ifndef HITBADGE_LED_H
#define HITBADGE_LED_H

#include <Arduino.h>

class LED{
  private:
    int pin;
    int brightness;
    int fadeAmount;
  public:
    LED(int P);
    void on();
    void off();
    void blink(int time);
    void fade();
};
#endif

//constructor
LED::LED(int P=13){
  pin = P; fadeAmount = 10; brightness = 0;
  pinMode(pin, OUTPUT);
}

void LED::on(){
  brightness = 255;
  analogWrite(pin, brightness);
  delay(10);
}

void LED::off(){
  brightness = 0;
  analogWrite(pin, brightness);
  delay(10);
}

void LED::blink(int time){
  on(); //digitalWrite(pin, HIGH);
  delay(time/2);
  off(); //digitalWrite(pin, LOW);
  delay(time/2);
}

void LED::fade(){
  analogWrite(pin, brightness);
  brightness = brightness + fadeAmount;
  if( brightness == 0 || brightness == 250){
    fadeAmount = -fadeAmount;
  }
}


  


