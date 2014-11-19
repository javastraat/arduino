// Project 2 - SOS Morse Code Signaler

int ledPin = 10;                // LED connected to digital pin 13

void setup()                    // run once, when the sketch starts
{
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
}

void loop()                     // run over and over again
{
  // 3 dits
  for (int x=0; x<3; x++) {
  digitalWrite(ledPin, HIGH);   // sets the LED on
  delay(150);                  // waits for 150ms
  digitalWrite(ledPin, LOW);    // sets the LED off
  delay(100);               // waits for 100ms
  }
  
  delay(100); // 100ms delay to cause slight gap between letters
    
    // 3 dahs
  for (int x=0; x<3; x++) {
  digitalWrite(ledPin, HIGH);   // sets the LED on
  delay(400);                  // waits for 400ms
  digitalWrite(ledPin, LOW);    // sets the LED off
  delay(100);               // waits for 100ms
  }
  
  delay(100); // 100ms delay to cause slight gap between letters
  
    // 3 dits again
  for (int x=0; x<3; x++) {
  digitalWrite(ledPin, HIGH);   // sets the LED on
  delay(150);                  // waits for 150ms
  digitalWrite(ledPin, LOW);    // sets the LED off
  delay(100);               // waits for 100ms
  }
  
  delay(5000); // wait 5 seconds before repeating the SOS signal
}





