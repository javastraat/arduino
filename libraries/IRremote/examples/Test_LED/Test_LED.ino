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
  
   if (results.value == 0)  // this is where you put in your IR remote button #
     {
       if (LEDon == true)   // is LEDon equal to true? 
         {
           LEDon = false;   
           digitalWrite(LED, HIGH);
           delay(100);      // keeps the transistion smooth
            
         }
         
        else
          {
            LEDon = true;
            digitalWrite(LED, LOW);
            delay(100);
            
          }
          
     }
  
    
    
}
