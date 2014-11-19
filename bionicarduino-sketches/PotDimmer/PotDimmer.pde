/*
 * PotDimmer 
 * ---------
 * 
 * Use a potentiometer to adjust the brightness of an PWM'd LED
 * 
 */

int potPin = 1;
int potPin1 = 2;    // select the input pin for the potentiometer
int ledPin = 5;   // select the pin for the LED
int ledPin1 = 6;
int val = 0;       // variable to store the value coming from the sensor
int val1 =0;

void setup() {
  pinMode(ledPin, OUTPUT);  // declare the ledPin as an OUTPUT
}

void loop() {
  val = analogRead(potPin);    // read the value from the sensor
  val = val/4;                 // convert from 0-1023 to 0-255
  analogWrite(ledPin, val);    // turn the ledPin on

  val1 = analogRead(potPin1);    // read the value from the sensor
  val1 = val1/4;                 // convert from 0-1023 to 0-255
  analogWrite(ledPin1, val1);    // turn the ledPin on

}
