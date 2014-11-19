#include <IRremote.h>
#include <IRremoteInt.h>

int message = 0;     //  This will hold one byte of the serial message
int ir_recv = 2; // assign ir single pin rec--5v-gnd
int ir_lednok = 3;
int ir_ledok = 5; // assign the pedestrian lights
int ob_led = 9;
int total = 0;
int key = 0;

IRrecv irrecv(ir_recv);
decode_results results;

void setup() {
  Serial.begin(9600);  //set serial to 9600 baud rate
  irrecv.enableIRIn(); // Start the receiver
  pinMode(ir_lednok, OUTPUT);
  pinMode(ir_ledok, OUTPUT);
  pinMode(ir_recv, INPUT);
  pinMode(ob_led, OUTPUT); 
  digitalWrite(ir_lednok, LOW); 
  digitalWrite(ir_lednok, LOW);
}


void loop() {

  if (irrecv.decode(&results)) {
    Serial.println(results.value, DEC);
    total = (results.value ++);
    key = (total + key);
    Serial.println(total, DEC);
    Serial.println(key, DEC);
    
    // flash the ir led 1 time
    for (int x=0; x<2; x++) {
      digitalWrite(ir_lednok, HIGH);
      delay(100);
      digitalWrite(ir_lednok, LOW);
      delay(100);

      irrecv.resume(); // Receive the next value

    }

  //  if (results.value == 33489661){
  if (total == -19891){  
    Serial.println("IR pressed");
     key=0;
      // flash the ir led 2 times
      for (int x=0; x<3; x++) {
        digitalWrite(ir_ledok, HIGH);
        delay(250);
        digitalWrite(ir_ledok, LOW);
        delay(250);

      }
    }

    //if (results.value == 4294967295){
    
    if (key == 18868){
    
      Serial.println("Stop pressing");

    }


  }
}









