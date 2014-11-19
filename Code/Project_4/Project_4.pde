// Project 4 - Interactive Traffic Lights

int carRed = 7; // assign the car lights
int carYellow = 8;
int carGreen = 9;
int pedRed = 6; // assign the pedestrian lights
int pedGreen = 5;
int SPKR =2;
int button = 3; // button pin
int crossTime = 10000; // time allowed to cross
unsigned long changeTime; // time since button pressed

void setup() {
  pinMode(carRed, OUTPUT);
  pinMode(carYellow, OUTPUT);
  pinMode(carGreen, OUTPUT);
  pinMode(pedRed, OUTPUT);
  pinMode(pedGreen, OUTPUT);
  pinMode(button, INPUT); // button on pin 2
  pinMode(SPKR, OUTPUT);
  // turn on the green light
  digitalWrite(carGreen, HIGH); 
  digitalWrite(pedRed, HIGH);
}

void loop() {
  
  digitalWrite(SPKR, HIGH); //turn the speaker on
   delay(1000);                     //wait for half a second
   digitalWrite(SPKR, LOW); //turn the speaker off
   delay(1000);                    //wait for half a second
   
  int state = digitalRead(button);
  /* check if button is pressed and it is 
  over 5 seconds since last button press */
  if (state == HIGH && (millis() - changeTime) > 5000) {
    // Call the function to change the lights
    changeLights();
  }
}

void changeLights() {
  digitalWrite(carGreen, LOW); // green off
  digitalWrite(carYellow, HIGH); // yellow on
  delay(2000); // wait 2 seconds
  
  digitalWrite(carYellow, LOW); // yellow off
  digitalWrite(carRed, HIGH); // red on
  delay(1000); // wait 1 second till its safe
  
  digitalWrite(pedRed, LOW); // ped red off
  digitalWrite(pedGreen, HIGH); // ped green on
  delay(crossTime); // wait for preset time period
  
  // flash the ped green
  for (int x=0; x<10; x++) {
    digitalWrite(pedGreen, HIGH);
    delay(250);
    digitalWrite(pedGreen, LOW);
    delay(250);
  }
  // turn ped red on
  digitalWrite(pedRed, HIGH);
  delay(500);
  
  delay(1000);
  digitalWrite(carRed, LOW);
  digitalWrite(carGreen, HIGH);
  
  // record the time since last change of lights
  changeTime = millis();
  // then return to the main program loop
}

