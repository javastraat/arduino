/*
Light up all the LED's in the matrix to test them
Nathan Clark

*/
int delayPin = 1;                         // delay before it turns the LED off


// pin[xx] on led matrix connected to nn on Arduino (-1 is dummy to make array start at pos 1)
int pins[17]= {-1, 5, 4, 3, 2, 14, 15, 16, 17, 13, 12, 11, 10, 9, 8, 7, 6};

// col[xx] of leds = pin yy on led matrix
int col[8] = {pins[13], pins[3], pins[4], pins[10], pins[06], pins[11], pins[15], pins[16]};

// row[xx] of leds = pin yy on led matrix
int row[8] = {pins[9], pins[14], pins[8], pins[12], pins[1], pins[7], pins[2], pins[5]};


void setup() {
  for (int thisPin = 0; thisPin < 8; thisPin++) {
    pinMode(col[thisPin], OUTPUT);      // Set selected pin to be an output rather then an input pin
    pinMode(row[thisPin], OUTPUT);       // this is so we cna provide +5v or GND at any pin
    digitalWrite(row[thisPin], HIGH);     // Set all cathodes to have positive voltage at them. Hence LED will not light up irrespective of what the anode is set to
  }
}

void loop() {
for (int x = 0;x <  500;x++); {
  onLED(0,0);
  onLED(1,1);
  onLED(0,1);
  onLED(1,0);
  onLED(6,0);
  onLED(7,1);
  onLED(6,1);
  onLED(7,0);
  onLED(3,3);
  onLED(4,3);
  onLED(3,2);
  onLED(4,2);
  onLED(0,5);
  onLED(1,6);
  onLED(2,7);
  onLED(3,7);
  onLED(4,7);
  onLED(5,7);
  onLED(6,6);
  onLED(7,5);
}
resetLEDS();
delay(500);

}
 
 void resetLEDS() {
   for (int thisPin = 0; thisPin < 8; thisPin++) {
    digitalWrite(row[thisPin], HIGH);     // Set all cathodes to have positive voltage at them. Hence LED will not light up irrespective of what the anode is set to
    digitalWrite(col[thisPin], LOW); 
  }
 } 
  
 void onLED(int colnum,int rownum) {
  digitalWrite(col[colnum], HIGH);
  digitalWrite(row[rownum], LOW);
  delay(delayPin);
  digitalWrite(col[colnum], LOW);
  digitalWrite(row[rownum], HIGH);
}

