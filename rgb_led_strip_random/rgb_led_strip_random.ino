// color swirl! connect an RGB LED to the PWM pins as indicated
// in the #defines
// public domain, enjoy!
#define REDPIN 7
#define GREENPIN 8
#define BLUEPIN 9
#define FADESPEED 1500 // make this higher to slow down
long randNumberi;
long randNumberii;
long randNumberiii;

void setup() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  Serial.begin(9600);
  //initial values (no significance)
  //int red = 255;
  //int blue = 255;
  //int green = 255;
  
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));

}
void loop() {
  // fade from blue to violet
 // analogWrite (REDPIN, red);
 // analogWrite (GREENPIN, green);
 // analogWrite (BLUEPIN, blue);
 // delay(10); //just to be safe

  //delay(FADESPEED);
  //analogWrite(REDPIN, 0);
  //analogWrite(GREENPIN, 0);
  //analogWrite(BLUEPIN, 255);
  
  // print a random number from 0 to 299
  randNumberi = random(256);
  randNumberii = random(256);
  randNumberiii = random(256);
  
  //Serial.println(randNumber);  

  // print a random number from 10 to 19
  //randNumber = random(10, 20);
  //Serial.println(randNumber);
  
  analogWrite (REDPIN, randNumberi);
  analogWrite (GREENPIN, randNumberii);
  analogWrite (BLUEPIN, randNumberiii);
  delay(FADESPEED);

}

