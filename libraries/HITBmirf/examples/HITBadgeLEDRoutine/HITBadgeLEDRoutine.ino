#include "HITBadge.h"

unsigned long time;
unsigned long backup_time;
HITBadge badge;

/// SETUP
void setup() { 

}

/// PROGRAM
void loop() {

  time = millis()/1000 - backup_time;
  if(time < 5){
    badge.blinkOne();
  }
  else if(time <10){
    badge.blinkTwo();
  }
  else if(time <15){
    badge.blinkThree();
  }
  else if(time <20){
    for(int i=0; i<3; i++)
      badge.blinkUp();
  }
  else if(time <25){
    for(int i=0; i<3; i++)
      badge.blinkRight();
  }
  else if(time <30){
    for(int i=0; i<3; i++)
      badge.blinkDown();
  }
  else if(time <35){
    for(int i =0; i<3; i++)
      badge.blinkLeft();
  }
  else if(time <40){
    for(int i =0; i<3; i++)
      badge.arrow();
  }
  else if(time <45){
    for(int i =0; i<3; i++)  
      badge.knightRider();
  }
  else if(time <50){
    for(int i =0; i<3; i++)
      badge.circleL();
  }
  else if(time <55){
    for(int i =0; i<3; i++)
      badge.circleR();
  }
  else if(time <60){
    for(int i =0; i<3; i++)
      badge.fill(1000);
  }
  else if(time <65){
    for(int i=0; i<3; i++)
      badge.flashAll();
  }
  else if(time <70){
    badge.fadeAll();
  }
  badge.reset();
  backup_time = time;
}

