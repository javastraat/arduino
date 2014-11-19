/*
Shield Layout
Pin 	# 	KEY
D0(RX) 	0 	NULL
D1(TX) 	1 	NULL
D2 	2 	KEY A
D3 	3 	KEY B
D4 	4 	KEY C
D5 	5 	KEY D
D6 	6 	KEY E
D7 	7 	KEY F
D8 	8 	KEY Down
Nokoia 5110 LCD
Pin 	# 	KEY
D9 	9 	SCLK
D10 	10 	CSN
D11 	11 	D/C
D12 	12 	RST
D13 	13 	SCE
nRF24L01
Pin 	# 	KEY
D9 	9 	SCE
D10 	10 	CSN
D11 	11 	SCK
D12 	12 	MOSI
D13 	13 	MISO 

  
  A basic sketch to demonstrate reading values from the joystick shield
  
  How to use:
  
    * Connect joystick shield to your Arduino
    * Upload this sketch to your Arduino
    * Open the Arduino IDE Serial Monitor (set to 9600 baud)
    * Waggle joystick, push buttons
  
  Requires:
  
    * Arduino
    * SparkFun Joystick Shield

  Written for SparkFun Arduino Inventor's Kit CIRC-JOY

  Based on original example by Ryan Owens

 */


// Store the Arduino pin associated with each input
const byte PIN_BUTTON_A = 2; // Select button is triggered when joystick is pressed
const byte PIN_BUTTON_B = 3;
const byte PIN_BUTTON_C = 4;
const byte PIN_BUTTON_D = 5;
const byte PIN_BUTTON_E = 6;
const byte PIN_BUTTON_F = 7;
const byte PIN_BUTTON_G = 8;


const byte PIN_ANALOG_X = 0;
const byte PIN_ANALOG_Y = 1;


void setup() {
  Serial.begin(9600);

  // Specify each pin connected to a pushbutton as an input.
  // Also enable the Arduino's internal "pull-up" resistors
  // for each pushbutton we want to read--this means the shield
  // doesn't need to have resistors on it.
  // Note that when a pull-up resistor is used on a pin the
  // meaning of the values read are reversed compared to their
  // usual meanings:
  //    * HIGH = the button is not pressed
  //    * LOW = the button is pressed  
  pinMode(PIN_BUTTON_A, INPUT);  
  digitalWrite(PIN_BUTTON_A, HIGH);

  pinMode(PIN_BUTTON_B, INPUT);  
  digitalWrite(PIN_BUTTON_B, HIGH);

  pinMode(PIN_BUTTON_C, INPUT);  
  digitalWrite(PIN_BUTTON_C, HIGH);

  pinMode(PIN_BUTTON_D, INPUT);  
  digitalWrite(PIN_BUTTON_D, HIGH);

  pinMode(PIN_BUTTON_E, INPUT);  
  digitalWrite(PIN_BUTTON_E, HIGH);

  pinMode(PIN_BUTTON_F, INPUT);  
  digitalWrite(PIN_BUTTON_F, HIGH);

  pinMode(PIN_BUTTON_G, INPUT);  
  digitalWrite(PIN_BUTTON_G, HIGH);
    
}


void loop() {
  // Print the current values of the inputs (joystick and
  // buttons) to the console.
  Serial.print("a:");
  Serial.print(digitalRead(PIN_BUTTON_A));
  Serial.print(" ");
  
  Serial.print("b:");
  Serial.print(digitalRead(PIN_BUTTON_B));
  Serial.print(" ");

  Serial.print("c:");
  Serial.print(digitalRead(PIN_BUTTON_C));
  Serial.print(" ");

  Serial.print("d:");
  Serial.print(digitalRead(PIN_BUTTON_D));
  Serial.print(" ");

  Serial.print("e:");
  Serial.print(digitalRead(PIN_BUTTON_E));
  Serial.print(" ");

  Serial.print("f:");
  Serial.print(digitalRead(PIN_BUTTON_F));
  Serial.print(" ");

  Serial.print("g:");
  Serial.print(digitalRead(PIN_BUTTON_G));
  Serial.print(" ");

  Serial.print("x:");
  Serial.print(analogRead(PIN_ANALOG_X));
  Serial.print(" ");
  
  Serial.print("y:");
  Serial.print(analogRead(PIN_ANALOG_Y));
  Serial.print(" ");  
  
  
  Serial.println();
}

