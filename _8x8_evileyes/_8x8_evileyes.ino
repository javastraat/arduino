/* During initiation, be sure to send every part to every max7219 and then
 upload it.
For example, if you have five max7219's, you have to send the scanLimit 5 times
before you load it-- other wise not every max7219 will get the data. the
function maxInUse  keeps track of this, just tell it how many max7219 you are
using.
*/

int dataIn = 10;
int load = 11;
int clock = 12;
int RandDelay;
int Action;

int maxInUse = 2;    //change this variable to set how many MAX7219's you'll use

int e = 0;	     // just a varialble

			   // define max7219 registers
byte max7219_reg_noop	  = 0x00;
byte max7219_reg_digit0	= 0x01;
byte max7219_reg_digit1	= 0x02;
byte max7219_reg_digit2	= 0x03;
byte max7219_reg_digit3	= 0x04;
byte max7219_reg_digit4	= 0x05;
byte max7219_reg_digit5	= 0x06;
byte max7219_reg_digit6	= 0x07;
byte max7219_reg_digit7	= 0x08;
byte max7219_reg_decodeMode  = 0x09;
byte max7219_reg_intensity   = 0x05;
byte max7219_reg_scanLimit   = 0x0b;
byte max7219_reg_shutdown    = 0x0c;
byte max7219_reg_displayTest = 0x0f;

void putByte(byte data) {
  byte i = 8;
  byte mask;
  while(i > 0) {
    mask = 0x01 << (i - 1);	// get bitmask
    digitalWrite( clock, LOW);   // tick
    if (data & mask){		// choose bit
	digitalWrite(dataIn, HIGH);// send 1
    }else{
	digitalWrite(dataIn, LOW); // send 0
    }
    digitalWrite(clock, HIGH);   // tock
    --i;				 // move to lesser bit
  }
}

void maxSingle( byte reg, byte col) {
//maxSingle is the "easy"  function to use for a     //single max7219

  digitalWrite(load, LOW);	 // begin
  putByte(1);			// specify register
  putByte(255);//((data & 0x01) * 256) + data >> 1); // put data
  digitalWrite(load, LOW);	 // and load da nuts
  digitalWrite(load,HIGH);
}

void maxAll (byte reg, byte col) {    // initialize  all  MAX7219's in the system
  int c = 0;
  digitalWrite(load, LOW);  // begin
  for ( c =1; c<= maxInUse; c++) {
  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
    }
  digitalWrite(load, LOW);
  digitalWrite(load,HIGH);

}

void setup () {


  pinMode(dataIn, OUTPUT);
  pinMode(clock,  OUTPUT);
  pinMode(load,   OUTPUT);

 

//initiation of the max 7219
  maxAll(max7219_reg_scanLimit, 0x07);
  maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  maxAll(max7219_reg_displayTest, 0x00); // no display test
   for (e=1; e<=8; e++) {    // empty registers, turn all LEDs off
    maxAll(e,0);
  }
  maxAll(max7219_reg_intensity, 0x05 & 0x05);    // the first 0x0f is the value you can set
								  // range: 0x00 to 0x0f
}

void loop () {
  Action = random(7);
  RandDelay = random(1000);

  if (Action == 1) {

  //Using a single Max7221 to light one led at a time

   maxAll(1,126);			     //  - + + + + + + -
   maxAll(2,255);			     //  + + + + + + + +
   maxAll(3,255);			     //  + + + + + + + +
   maxAll(4,231);			     //  + + + - - + + +
   maxAll(5,231);			     //  + + + - - + + +
   maxAll(6,255);			     //  + + + + + + + +
   maxAll(7,255);			     //  + + + + + + + +
   maxAll(8,126);			     //  - + + + + + + -

  delay(RandDelay);
  }
  if (Action == 2) {
   
   maxAll(1,126);			     //  - + + + + + + -
   maxAll(2,255);			     //  + + + + + + + +
   maxAll(3,231);			     //  + + + - - + + +
   maxAll(4,231);			     //  + + + - - + + +
   maxAll(5,255);			     //  + + + + + + + +
   maxAll(6,255);			     //  + + + + + + + +
   maxAll(7,255);			     //  + + + + + + + +
   maxAll(8,126);			     //  - + + + + + + -

delay (RandDelay); 
  }
  if (Action == 3) {

   maxAll(1,126);			     //  - + + + + + + -
   maxAll(2,255);			     //  + + + + + + + +
   maxAll(3,255);			     //  + + + + + + + +
   maxAll(4,255);			     //  + + + + + + + +
   maxAll(5,231);			     //  + + + - - + + +
   maxAll(6,231);			     //  + + + - - + + +
   maxAll(7,255);			     //  + + + + + + + +
   maxAll(8,126);			     //  - + + + + + + -

  delay(RandDelay);  
  }
  if (Action == 4) { 

   maxAll(1,126);			     //  - + + + + + + -
   maxAll(2,255);			     //  + + + + + + + +
   maxAll(3,159);			     //  + + + + + - - +
   maxAll(4,159);			     //  + + + + + - - +
   maxAll(5,255);			     //  + + + + + + + +
   maxAll(6,255);			     //  + + + + + + + +
   maxAll(7,255);			     //  + + + + + + + +
   maxAll(8,126);			     //  - + + + + + + -
 
  delay(RandDelay);
  }      
  if (Action == 5) {
  
   maxAll(1,120);			     //  - - - + + + + -
   maxAll(2,252);			     //  - - + + + + + +
   maxAll(3,252);			     //  - - + + + + + +
   maxAll(4,204);			     //  - - + + - - + +
   maxAll(5,204);			     //  - - + + - - + +
   maxAll(6,252);			     //  - - + + + + + +
   maxAll(7,252);			     //  - - + + + + + +
   maxAll(8,120);			     //  - - - + + + + -

delay (50); 

   maxAll(1,0);	         		     //  - - - - - - - -
   maxAll(2,192);			     //  - - - - - - + +
   maxAll(3,192);			     //  - - - - - - + +
   maxAll(4,192);			     //  - - - - - - + +
   maxAll(5,192);			     //  - - - - - - + +
   maxAll(6,192);			     //  - - - - - - + +
   maxAll(7,192);			     //  - - - - - - + +
   maxAll(8,0);			             //  - - - - - - - -

  delay(500);
 
   maxAll(1,120);			     //  - - - + + + + -
   maxAll(2,252);			     //  - - + + + + + +
   maxAll(3,252);			     //  - - + + + + + +
   maxAll(4,204);			     //  - - + + - - + +
   maxAll(5,204);			     //  - - + + - - + +
   maxAll(6,252);			     //  - - + + + + + +
   maxAll(7,252);			     //  - - + + + + + +
   maxAll(8,120);			     //  - - - + + + + -

delay (50);  
  
   maxAll(1,126);			     //  - + + + + + + -
   maxAll(2,255);			     //  + + + + + + + +
   maxAll(3,255);			     //  + + + + + + + +
   maxAll(4,231);			     //  + + + - - + + +
   maxAll(5,231);			     //  + + + - - + + +
   maxAll(6,255);			     //  + + + + + + + +
   maxAll(7,255);			     //  + + + + + + + +
   maxAll(8,126);			     //  - + + + + + + -
   
delay (RandDelay);
  }

  if (Action == 6) {
   maxAll(1,126);			     //  - + + + + + + -
   maxAll(2,207);			     //  + + + + + - - +
   maxAll(3,207);			     //  + + + + + - - +
   maxAll(4,255);			     //  + + + + + + + +
   maxAll(5,255);			     //  + + + + + + + +
   maxAll(6,255);			     //  + + + + + + + +
   maxAll(7,255);			     //  + + + + + + + +
   maxAll(8,126);			     //  - + + + + + + -
     
delay (RandDelay);
  }
  
 if (Action == 7) {
   maxAll(1,126);			     //  - + + + + + + -
   maxAll(2,255);			     //  + + + + + + + +
   maxAll(3,255);			     //  + + + + + + + +
   maxAll(4,255);			     //  + + + + + + + +
   maxAll(5,255);			     //  + + + + + + + +
   maxAll(6,207);			     //  + + + + + - - +
   maxAll(7,207);			     //  + + + + + - - +
   maxAll(8,126);			     //  - + + + + + + -
   
delay (RandDelay);

}
}

