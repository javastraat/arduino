#ifndef LED_H
#define LED_H

#include <Arduino.h>

class LED{
  private:
    int pin;
    int brightness;
    int fadeAmount;
  public:
    LED(int P);
    int getPin();          //is for debugging purpose
    int getBrightness();   //is for debugging purpose
    int getFadeAmount();   //is for debugging purpose
    void on();
    void off();
    void blink(int time);
    void fade();
    void reset();
};
#endif

//constructor
LED::LED(int P = 13){
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

int LED::getPin(){
  return pin;
}

int LED::getBrightness(){
  return brightness;
}

int LED::getFadeAmount(){
  return fadeAmount;
}  

void LED::reset(){
  brightness = 0;
  analogWrite(pin, brightness);
}
  

