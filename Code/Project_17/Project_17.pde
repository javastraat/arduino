// Project 17 - LED DOt Matrix - Basic Animation

#include <TimerOne.h>

//Pin connected to Pin 12 of 74HC595 (Latch)
int latchPin = 7;
//Pin connected to Pin 11 of 74HC595 (Clock)
int clockPin = 8;
//Pin connected to Pin 14 of 74HC595 (Data)
int dataPin = 4;

uint8_t led[8];
long counter1 = 0;
long counter2 = 0;

void setup() {
  //set pins to output 
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  //digitalWrite(latchPin, HIGH);
  led[0] = B01000100;
  led[1] = B11101110;
  led[2] = B11111110;
  led[3] = B11111110;
  led[4] = B11111110;
  led[5] = B01111100;
  led[6] = B00111000;
  led[7] = B00010000;
  Timer1.initialize(10000);
  Timer1.attachInterrupt(screenUpdate);
}

void loop() {
  counter1++;
    if (counter1 >=100000) {counter2++;}
if (counter2 >= 10000) {
  counter1 = 0;
  counter2 = 0;
  for (int i=0; i<8; i++) {
    led[i]= ~led[i];
  }
  }
}



void screenUpdate() {
uint8_t row = B00000001;
 for (byte k = 0; k < 9; k++) {
    digitalWrite(latchPin, LOW); // Open up the latch ready to receive data

      shiftIt(~row ); 
      shiftIt(led[k] ); // LED array

   digitalWrite(latchPin, HIGH); // Close the latch, sending the data in the registers out to the matrix
    row = row << 1;
 }

}


void shiftIt(byte dataOut) {
  // Shift out 8 bits LSB first, 
  // on rising edge of clock

  boolean pinState;

  //clear shift register read for sending data
  digitalWrite(dataPin, LOW);

  // for each bit in dataOut send out a bit
  for (int i=7; i>=0; i--)  {
      //set clockPin to LOW prior to sending bit
    digitalWrite(clockPin, LOW);

    // if the value of DataOut and (logical AND) a bitmask
    // are true, set pinState to 1 (HIGH)
    if ( dataOut & (1<<i) ) {
      pinState = HIGH;
    }
    else {	
      pinState = LOW;
    }

    //sets dataPin to HIGH or LOW depending on pinState
    digitalWrite(dataPin, pinState);
    //send bit out on rising edge of clock 
    digitalWrite(clockPin, HIGH);
    digitalWrite(dataPin, LOW);
  }

  //stop shifting
  digitalWrite(clockPin, LOW);
}

