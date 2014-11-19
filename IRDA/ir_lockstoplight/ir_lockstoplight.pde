#include <IRremote.h>
#include <IRremoteInt.h>

int message = 0;       // This will hold one byte of the serial message
int ir_recv = 2;       // assign ir single pin rec--5v-gnd
int ledred = 3;     // Not OK Led (red)
int ledgreen = 5;      // OK Led (green)
int lockred = 6;
int lockyellow = 7;
int lockgreen = 8;
int ob_led = 9;        // onboard led if we need it
int ircode = 0;        // calculation of ir code
int code = 0;
int key = 19120;           // key value
int keycode = -32512;   // 1-0 11980; //1-4 18868;
int keycode2 = 12137;  //second code
int tempkey = 11223;    // any value to miscalc first try after boot (reset first)
int wrongkey = 11223;
int resetkey = 2704; // reset ir code
int resetkey1 = -19891;   // reset ir code 2
int fakekey = 11223;
long randNumber;       // randomnumber space

IRrecv irrecv(ir_recv); // irreciever
decode_results results; // decode ir results

void setup() {
  Serial.begin(9600);          //set serial to 9600 baud rate
  irrecv.enableIRIn();         // Start the ir receiver
  randomSeed(analogRead(0));   // use a0 for noise data (dont connect anything to a0)
  pinMode(lockred, OUTPUT);
  pinMode(lockyellow, OUTPUT);
  pinMode(lockgreen, OUTPUT);
  pinMode(ledred, OUTPUT);  // define ports
  pinMode(ledgreen, OUTPUT);
  pinMode(ir_recv, INPUT);
  pinMode(ob_led, OUTPUT); 
}


void loop() {

  if (irrecv.decode(&results)) { 

    ircode = (results.value++);
    code = (ircode * ircode);
    fakekey = (code + tempkey * code + tempkey);
    key = fakekey;

    Serial.print("Key : ");
    Serial.println(key, DEC);
    Serial.print("Fakekey : ");
    Serial.println(fakekey, DEC);
    Serial.print("IR Code : ");
    Serial.println(results.value, DEC);
    Serial.print("IR Code calculation : ");
    Serial.println(ircode, DEC);
    Serial.print("Tempkey : ");
    Serial.println(tempkey, DEC);
    Serial.print("Keycode : ");
    Serial.println(keycode, DEC);
    Serial.print("Keycode2 : ");
    Serial.println(keycode2, DEC);
    Serial.println("");
    Serial.println("-------------------------------------------");
    Serial.println("");


    // flash the ir led 1 time
    for (int x=0; x<1; x++) {
      digitalWrite(ledred, HIGH);
      delay(100);
      digitalWrite(ledred, LOW);
      delay(100);

      irrecv.resume(); // Receive the next value

    }
  }

  //  if (results.value == 33489661){
  if (ircode == resetkey || ircode == resetkey1){  
    Serial.println("Reset");
    Serial.println("");
    Serial.println("-------------------------------------------");
    Serial.println("");
    tempkey=0;

    // flash the onboard led 3 times
    for (int x=0; x<3; x++) {
      digitalWrite(ob_led, HIGH);
      delay(100);
      digitalWrite(ob_led, LOW);
      delay(100);

    }

  }


  //  if (results.value == 33489661){
  if (key == keycode || key == keycode2){  
    Serial.println("KEY found");
    Serial.println("");
    Serial.println("Lock open :)");
    Serial.println("");
    //randNumber = random(3000);
    // tempkey = randNumber;
    tempkey = random(3000);
    key = random(3000);
    fakekey = random(3000);
    Serial.print("New key : ");
    Serial.println(key, DEC);
    Serial.println("");
    Serial.print("New Tempkey : ");
    Serial.println(tempkey, DEC);
    Serial.println("");
    Serial.print("New Fakekey : ");
    Serial.println(fakekey, DEC);
    Serial.println("");
    Serial.println("-------------------------------------------");
    digitalWrite(ledgreen, HIGH);

    // flash the ir led 3 times
    for (int x=0; x<3; x++) {
      digitalWrite(lockyellow, HIGH);
      delay(250);
      digitalWrite(lockyellow, LOW);
      delay(250);

    }
    digitalWrite(lockred, LOW);

    // flash the ir led 3 times
    for (int x=0; x<4; x++) {
      digitalWrite(lockgreen, HIGH);
      delay(1000);
      digitalWrite(lockgreen, LOW);
      delay(1000);
    }

    digitalWrite(lockred, LOW);
    digitalWrite(lockyellow, HIGH);
    digitalWrite(lockgreen, HIGH);
    digitalWrite(ledgreen, LOW);
    delay(250); 

  }

  //  if (results.value == 33489661){
  if (tempkey == wrongkey || fakekey==wrongkey){  
    Serial.println("Tempkey not valid after boot :)");
    Serial.println("");
    Serial.println("-------------------------------------------");
    Serial.println("");
    tempkey = random(3000);
    key = random(3000);
    fakekey = random(3000);
    Serial.print("New key : ");
    Serial.println(key, DEC);
    Serial.println("");
    Serial.print("New Tempkey : ");
    Serial.println(tempkey, DEC);
    Serial.println("");
    Serial.print("New Fakekey : ");
    Serial.println(fakekey, DEC);
    Serial.println("");
    Serial.println("-------------------------------------------");
    digitalWrite(lockgreen, HIGH);
    digitalWrite(lockyellow, HIGH);

    // flash the onboard led 3 times
    for (int x=0; x<3; x++) {
      digitalWrite(ob_led, HIGH);
      delay(100);
      digitalWrite(ob_led, LOW);
      delay(100);
      digitalWrite(lockred, HIGH);
      delay(100);
      digitalWrite(lockred, LOW);
      delay(100);

    }
  }
}
























