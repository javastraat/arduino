/*
 * Arduino XMAS Blog hitcounter
 * http://tinkerlog.com/
 *
 * Reference:
 * - Servo control from an analog input
 *   taken from http://itp.nyu.edu/physcomp/Labs/Servo
 *
 */
int servoPin = 9;      // control pin for servo motor
int minPulse = 1200;   // minimum servo position
int maxPulse = 1700;   // maximum servo position
int pulse = 0;         // amount to pulse the servo
int rings = 0;         // amount of rings
long nextMillis = 0;   // the time to pass before the next action
long lastPulse = 0;    // the time in milliseconds of the last pulse
int refreshTime = 20;  // the time needed in between pulses
int state = 0;         // for the state machine

void setup() {
  pinMode(servoPin, OUTPUT);  // Set servo pin as an output pin
  pulse = minPulse;           // Set the motor position value to the minimum
  Serial.begin(9600);
}

// define states for the state machine
#define PULSE_ON       0
#define WAIT_PULSE_ON  1
#define PULSE_OFF      2
#define WAIT_PULSE_OFF 3

void loop() {

  // read the rings
  if (Serial.available() > 0) {
    rings += Serial.read();
    Serial.println(rings);
  }

  // state machine to control, which action to perform
  if (rings > 0) {
    switch (state) {
      case PULSE_ON:
        pulse = maxPulse;                // send servo to max position
        nextMillis = millis() + 300;     // wait 300 ms
        state = WAIT_PULSE_ON;
        break;
      case WAIT_PULSE_ON:
        if (millis() > nextMillis) {
          state = PULSE_OFF;             // time is up
        }
        break;
      case PULSE_OFF:
        pulse = minPulse;                // send servo to min position
        nextMillis = millis() + 1000;    // wait 1 second between two rings
        state = WAIT_PULSE_OFF;
        break;
      case WAIT_PULSE_OFF:
        if (millis() > nextMillis) {
          state = PULSE_ON;              // time is up
          rings--;                       // one ring is done
        }
        break;
      }
  }

  // pulse the servo again if the refresh time (20 ms) have passed:
  if (millis() - lastPulse >= refreshTime) {
    digitalWrite(servoPin, HIGH);   // Turn the motor on
    delayMicroseconds(pulse);       // Length of the pulse sets the motor position
    digitalWrite(servoPin, LOW);    // Turn the motor off
    lastPulse = millis();           // save the time of the last pulse
  }

}
