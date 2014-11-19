// Project 6 - Interactive LED Chase Effect

// Create array for LED pins
byte ledPin[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
int ledDelay; // delay betwen changes
int direction = 1;
int currentLED = 0;
unsigned long changeTime;
int potPin = 2;    // select the input pin for the potentiometer

void setup() {
  // set all pins to output
  for (int x=0; x<10; x++) {
    pinMode (ledPin[x], OUTPUT); }
    changeTime = millis();
}

void loop() {
  ledDelay = analogRead(potPin);    // read the value from the pot
  // if it has been ledDelay ms since last change
  if ((millis() - changeTime) > ledDelay) {
    changeLED();
    changeTime = millis();
  }
}

void changeLED() {
  // turn off all LED's
  for (int x=0; x<10; x++) {
    digitalWrite(ledPin[x], LOW);
  }
  // turn on the current LED
  digitalWrite(ledPin[currentLED], HIGH);
  // increment by the direction value
  currentLED += direction;
  // change direction if we reach the end
  if (currentLED == 9) {direction = -1;}
  if (currentLED == 0) {direction = 1;}
}




