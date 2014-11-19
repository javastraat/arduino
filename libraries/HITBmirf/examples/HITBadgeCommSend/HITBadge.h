#ifndef HITBADGE_H 
#define HITBADGE_H

#include <Arduino.h>
#include "HITBadge_led.h"

//#define true 1
//#define false 0

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
    void flashAll();
    void knightRider(int);
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
  for(int i = 0; i < 4; ++i)
    array[i].fade();
  delay(50);
}

void HITBadge::flashAll(){
  for(int i=0; i<4; ++i)
    array[i].on();
  delay(100);
  for(int i=0; i<4; ++i)
    array[i].off();  
  delay(100);
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
  delay(time);
  for(int i=0; i<5; ++i){
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
