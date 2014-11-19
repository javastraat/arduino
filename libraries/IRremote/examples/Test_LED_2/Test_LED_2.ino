/*
 Some Sample code of how to use your IR remote
 
 * Lets get started:
 
 The IR sensor's pins are attached to Arduino as so:
 Pin 1 to Vout (pin 11 on Arduino)
 Pin 2 to GND
 Pin 3 to Vcc (+5v from Arduino)

*/

#include <IRremote.h>

int IRpin = 11;  // pin for the IR sensor
int LED = 13;    // LED pin  
IRrecv irrecv(IRpin);
decode_results results;

boolean LEDon = true; // initializing LEDon as true

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(LED, OUTPUT);
}

void loop() 
{
   
  if (irrecv.decode(&results)) 
    {
      
      irrecv.resume();   // Receive the next value
    }
  
  switch(results.value)
 {

  case 16753245:
  // do this
  break;
  
  case 02:
  // do another thing
  break;
  
  case 03:
  // feed my dog for me
  break;
  
  default:
  digitalWrite(LED, HIGH);
  
  }
  
}
