#include <IRremote.h>
#include <IRremoteInt.h>

int message = 0;     //  This will hold one byte of the serial message
int ir_recv = 2; // assign ir single pin rec--5v-gnd
int ir_lednok = 3;
int ir_ledok = 4; // assign the pedestrian lights
int pedGreen = 5;
int carRed = 6; // assign the car lights
int carYellow = 7;
int carGreen = 8;
int button = 9; // button pin
int crossTime = 5000; // time allowed to cross
unsigned long changeTime; // time since button pressed
IRrecv irrecv(ir_recv);
decode_results results;

void setup() {
  Serial.begin(9600);  //set serial to 9600 baud rate
  irrecv.enableIRIn(); // Start the receiver
  pinMode(carRed, OUTPUT);
  pinMode(carYellow, OUTPUT);
  pinMode(carGreen, OUTPUT);
  pinMode(pedRed, OUTPUT);
  pinMode(pedGreen, OUTPUT);
  pinMode(ir_led, OUTPUT);
  pinMode(ir_recv, INPUT);
  pinMode(button, INPUT); 
  digitalWrite(carGreen, HIGH); 
  digitalWrite(pedRed, HIGH);
}


void loop() {
  if (Serial.available() > 0) { //  Check if there is a new message
    message = Serial.read();    //  Put the serial input into the message

    if (message == 'R'){  //  If a capitol R is received...
      Serial.println("Operator RED");
      changeLights();       //  change lights  
    }
  }

  int state = digitalRead(button);
  /* check if button is pressed and it is 
   over 5 seconds since last button press */
  if (state == HIGH && (millis() - changeTime) > 5000) {
    // Call the function to change the lights
    Serial.println("Button pressed");
    changeLights();
  }

  if (irrecv.decode(&results)) {
    Serial.println(results.value, DEC);

    // flash the ir led 1 time
    for (int x=0; x<1; x++) {
      digitalWrite(ir_led, HIGH);
      delay(100);
      digitalWrite(ir_led, LOW);
      delay(100);

      irrecv.resume(); // Receive the next value

    }

    if (results.value == 33489661){
      Serial.println("IR pressed");

      // flash the ir led 2 times
      for (int x=0; x<2; x++) {
        digitalWrite(ir_led, HIGH);
        delay(250);
        digitalWrite(ir_led, LOW);
        delay(250);

      }
      changeLights();
    }

    if (results.value == 4294967295){
      Serial.println("Stop pressing");

    }
  }

}


void changeLights() {
  digitalWrite(carGreen, LOW); // green off
  digitalWrite(carYellow, HIGH); // yellow on
  delay(2000); // wait 2 seconds

  digitalWrite(carYellow, LOW); // yellow off
  digitalWrite(carRed, HIGH); // red on
  delay(2000); // wait 2 seconds till its safe

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
  delay(2000);
  // turn car green
  digitalWrite(carRed, LOW);
  digitalWrite(carGreen, HIGH);

  // record the time since last change of lights
  changeTime = millis();
  // then return to the main program loop
}






