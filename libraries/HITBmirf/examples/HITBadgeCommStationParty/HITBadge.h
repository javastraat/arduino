#ifndef HITBADGE_H 
#define HITBADGE_H

#include <Arduino.h>
#include "HITBadge_led.h"

class HITBadge{
  private:
    LED led1;
    LED led2;
    LED led3;
    LED led4;
    LED pin13;
    LED array[5];    
    boolean toggle;
  public:
    HITBadge();
    void fadeAll();
    void flashAll(int);
    void blinkRight(int);
    void blinkLeft(int);
    void blinkUp(int);
    void blinkDown(int);
    void blinkOne(int);
    void blinkTwo(int);
    void blinkThree(int);
    void knightRider(int);
    void circle(int);
    void circleR(int);
    void circleL(int);
    void arrow(int);
    void fill(int);
    void reset();
    void cpuPulse();
};
#endif

HITBadge::HITBadge():led1(4), led2(6), led3(3), led4(5) {
    array[0] = led1;
    array[1] = led2;
    array[2] = led3;
    array[3] = led4;
    array[4] = pin13;
    toggle = false;
}

void HITBadge::fadeAll(){
  for(int i = 1; i < 4; ++i)
    array[i].fade();
  delay(50);
}

void HITBadge::flashAll(int time=200){
  for(int i=0; i<4; ++i)
    array[i].on();
  delay(time/2);
  for(int i=0; i<4; ++i)
    array[i].off();  
  delay(time/2);
}

void HITBadge::blinkRight(int time=100){
  array[0].on();
  array[1].on();
  array[2].on();
  delay(time/2);
  array[0].off();
  array[1].off();
  array[2].off();
  delay(time/2);  
}

void HITBadge::blinkLeft(int time=100){
  array[0].on();
  array[3].on();
  array[2].on();
  delay(time/2);
  array[0].off();
  array[3].off();
  array[2].off();
  delay(time/2);  
}

void HITBadge::blinkUp(int time=100){
  array[3].on();
  array[0].on();
  array[1].on();
  delay(time/2);
  array[3].off();
  array[0].off();
  array[1].off();
  delay(time/2);  
}

void HITBadge::blinkDown(int time=100){
  array[1].on();
  array[2].on();
  array[3].on();
  delay(time/2);
  array[1].off();
  array[2].off();
  array[3].off();
  delay(time/2);   
}

void HITBadge::blinkOne(int time=150){
  array[1].on();
  delay(time/2);
  array[1].off();
  delay(time/2);  
}

void HITBadge::blinkTwo(int time=150){
  array[1].on();
  array[2].on();
  delay(time/2);
  array[1].off();
  array[2].off();
  delay(time/2);  
}

void HITBadge::blinkThree(int time=150){
  array[1].on();
  array[2].on();
  array[3].on();
  delay(time/2);
  array[1].off();
  array[2].off();
  array[3].off();
  delay(time/2);  
}

void HITBadge::knightRider(int time=20){
  for(int i=1; i<4; ++i){
    array[i].on();
    delay(time);
    array[i+1].on();
    delay(time);
    array[i].off();
    delay(time*2);
  }
  for(int i=3; i>1; --i){
    array[i].on();
    delay(time);
    array[i-1].on();
    delay(time);
    array[i].off();
    delay(time*2);
  }
}

void HITBadge::circle(int time=150){
  circleR(time);
}

void HITBadge::circleR(int time=150){
  for(int i = 0; i < 4; ++i){
    array[i].blink(time);
    if(i==3)
      array[0].blink(time);
  }
}

void HITBadge::circleL(int time=150){
  for(int i = 3; i >= 0; --i){
    if(i==3)
      array[0].blink(time);
    array[i].blink(time);
  }
}

void HITBadge::arrow(int time = 100){
  array[5].blink(time);
  array[4].blink(time);
  array[0].blink(time);
  array[2].on();
  delay(time/2);
  array[1].on();
  array[3].on();
  delay(time/2);
  array[2].off();
  delay(time/2);  
  array[1].off();
  array[3].off();
}

void HITBadge::fill(int time = 200){
  array[2].on();
  delay(time/2);
  array[1].on();
  array[3].on();
  delay(time/2);
  array[0].on();
  delay(time/2);
  array[4].on();
  delay(time/2);
  array[5].on();
  delay(time);
  for(int i=0; i<6; ++i){
    array[i].off();
  }
  delay(time);
}

void HITBadge::reset(){
  for(int i = 0; i < 4; ++i){
    array[i].off();
  }
}

void HITBadge::cpuPulse(){
  if(!toggle){
    pin13.on();
    toggle = true;
  }else{
    pin13.off();
    toggle = false;
  }
}
