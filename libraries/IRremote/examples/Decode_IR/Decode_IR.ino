/*
 * Sketch modified by Enjoying Electronics: http://www.instructables.com/member/Enjoying+Electronics/
 Code based off of: 
 * IRremote
 * Version 0.1 July, 2009 
 * Copyright 2009 Ken Shirriff
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
 
 * Special thanks to dablondeemu http://www.instructables.com/member/dablondeemu/
 * and his instructable listed below, IR Remote Controlled Color Changing Cloud (Arduino) 
 * http://www.instructables.com/id/IR-Remote-Controlled-Color-Changing-Cloud-Arduino/  
 
 
 * Lets get started:
 
 The IR sensor's pins are attached to Arduino as so:
 Pin 1 to Vout (pin 11 on Arduino)
 Pin 2 to GND
 Pin 3 to Vcc (+5v from Arduino)

*/

/*******************CODE BEGINS HERE********************/

#include <IRremote.h>

int IRpin = 11;
IRrecv irrecv(IRpin);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() 
{
  if (irrecv.decode(&results)) 
    {
      Serial.println(results.value, DEC); // Print the Serial 'results.value'
      irrecv.resume();   // Receive the next value
    }
  
  
}
