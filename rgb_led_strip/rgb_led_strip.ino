    // color swirl! connect an RGB LED to the PWM pins as indicated
    // in the #defines
    // public domain, enjoy!
    #define REDPIN 7
    #define GREENPIN 8
    #define BLUEPIN 9
    #define FADESPEED 10 // make this higher to slow down
    void setup() {
    pinMode(REDPIN, OUTPUT);
    pinMode(GREENPIN, OUTPUT);
    pinMode(BLUEPIN, OUTPUT);
    }
    void loop() {
    int r, g, b;
    // fade from blue to violet
    for (r = 0; r < 256; r++) {
    analogWrite(REDPIN, r);
    delay(FADESPEED);
    }
    // fade from violet to red
    for (b = 255; b > 0; b--) {
    analogWrite(BLUEPIN, b);
    delay(FADESPEED);
    }
    // fade from red to yellow
    for (g = 0; g < 256; g++) {
    analogWrite(GREENPIN, g);
    delay(FADESPEED);
    }
    // fade from yellow to green
    for (r = 255; r > 0; r--) {
    analogWrite(REDPIN, r);
    delay(FADESPEED);
    }
    // fade from green to teal
    for (b = 0; b < 256; b++) {
    analogWrite(BLUEPIN, b);
    delay(FADESPEED);
    }
    // fade from teal to blue
    for (g = 255; g > 0; g--) {
    analogWrite(GREENPIN, g);
    delay(FADESPEED);
    }
    }
