#include "LED.h"

//#define serialPrint 

LED led1(4);
LED led2(6);
LED led3(3);
LED led4(5);
LED array[4] = {led1, led2, led3, led4};

int cpu = 13;

unsigned long time;

boolean toggle = false;

/// SETUP
void setup() { 
  Serial.begin(9600);  
  pinMode(cpu, OUTPUT);
}

/// PROGRAM
void loop() {
 
  time = millis()/1000;

  #ifdef serialPrint
  printLed();
  #endif
 
  if(time < 10){
    fadeAll();
  }
  else if(time < 20){
    blinkCircle();
  }
  else{
    reset();
  }

  cpuPulse();
}
 

/// FUNCTIONS
void printLed(){
  Serial.println("+++++++++++++++++++++++++");
  Serial.println("-------------------------");
  for(int i = 0; i < 4; ++i){
    Serial.print("led: "); Serial.println(array[i].getPin());
    Serial.print("\tbrightness: "); Serial.println(array[i].getBrightness());
    Serial.print("\tfadeAmount: "); Serial.print(array[i].getFadeAmount()); Serial.print(" "); Serial.println(array[i].getFadeAmount(), BIN);
    Serial.println("-------------------------");
  }
  Serial.println("+++++++++++++++++++++++++");
}

void fadeAll(){
  for(int i = 0; i < 4; ++i){
    array[i].fade();
  }
  delay(50);
}

void blinkCircle(){
  for(int i = 1; i < 4; ++i){
    array[i].blink(100);
  }
  for(int i = 3; i > 0; --i){
    array[i].blink(100);
  }
}

void reset(){
  for(int i = 0; i < 4; ++i){
    array[i].reset();
  }
}

void cpuPulse(){
  if(!toggle){
    digitalWrite(cpu, HIGH);
    toggle = true;
  }else{
    digitalWrite(cpu, LOW);
    toggle = false;
  }
}
