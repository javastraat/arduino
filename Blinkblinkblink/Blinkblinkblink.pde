/*
  Blink
 Turns on an LED on for one second, then off for one second, repeatedly.
 
 This example code is in the public domain.
 */

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(3, OUTPUT);     
  pinMode(4, OUTPUT);     
  pinMode(5, OUTPUT);     
  pinMode(6, OUTPUT);     
  pinMode(7, OUTPUT);     
  pinMode(8, OUTPUT);     
  pinMode(9, OUTPUT);     
}

void loop() {
  digitalWrite(3, HIGH);   // set the LED on
  digitalWrite(4, HIGH);   // set the LED on
  digitalWrite(5, HIGH);   // set the LED on
  digitalWrite(6, HIGH);   // set the LED on
  digitalWrite(7, HIGH);   // set the LED on
  digitalWrite(8, HIGH);   // set the LED on
  digitalWrite(9, HIGH);   // set the LED on
  delay(2000);              // wait for a second

  digitalWrite(3, LOW);    // set the LED off
  digitalWrite(4, LOW);    // set the LED off
  digitalWrite(5, LOW);    // set the LED off
  digitalWrite(6, LOW);    // set the LED off
  digitalWrite(7, LOW);    // set the LED off
  digitalWrite(8, LOW);    // set the LED off
  digitalWrite(9, LOW);    // set the LED off
  delay(2000);              // wait for a second

}




