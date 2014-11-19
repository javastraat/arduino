
#include <LED7Segment.h>

const int latchPin = 7; //Pin connected to ST_CP of 74HC595
const int clockPin = 8; //Pin connected to SH_CP of 74HC595
const int dataPin = 4; //Pin connected to DS of 74HC595

void setup() {
 //set pins to output because they are addressed in the main loop
 pinMode(latchPin, OUTPUT);
 pinMode(clockPin, OUTPUT);
 pinMode(dataPin, OUTPUT);
}

void loop() {
 //count up routine
 for (int j = 0; j < 128; j++) {
 display(j/10, j - (j/10) * 10);
 delay(300);
 }
}

void display(byte seg1, byte seg2){
 send(seg1);
 send(seg2);
}

void send(byte data){
 //ground latchPin and hold low for as long as you are transmitting
 digitalWrite(latchPin, LOW);
 shiftOut(dataPin, clockPin, LSBFIRST, ledCharSet[data]);
 //return the latch pin high to signal chip that it no longer needs to listen for information
 digitalWrite(latchPin, HIGH);
}

