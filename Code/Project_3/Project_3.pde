// Project 3 - Traffic Lights

int ledDelay = 5000; // delay in between changes
int redPin = 2;
int yellowPin = 3;
int greenPin = 5;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
}

void loop() {
  
  digitalWrite(redPin, HIGH); // turn the red light on
  delay(ledDelay); // wait 5 seconds
  
  digitalWrite(redPin, LOW); // turn the red light off
  digitalWrite(greenPin, HIGH); // turn on green
  delay(ledDelay); // wait 2 seconds
  
  digitalWrite(yellowPin, HIGH); // turn yellow off
  delay(2000); // wait ledDelay milliseconds
  
  digitalWrite (yellowPin, LOW); // turn yellow on
  digitalWrite(greenPin, LOW); // turn on green
  digitalWrite(redPin, HIGH); // turn green off
  delay(ledDelay); // wait 2 seconds
  
  // now our loop repeats
  
}


