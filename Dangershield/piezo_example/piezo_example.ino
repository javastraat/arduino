/*************************************************************************************************
* This file contains code and definitions to make your life using the Danger Shield really easy.
**************************************************************************************************/

//these are our digital input pins
#define BUTTON1_PIN 2
#define BUTTON2_PIN 7
#define BUTTON3_PIN 4

//these are our analog input pins
#define SLIDER1_PIN 2
#define SLIDER2_PIN 1
#define SLIDER3_PIN 0
#define LIGHT_SENSOR_PIN 3
#define TEMP_SENSOR_PIN 4
#define KNOCK_SENSOR_PIN 5

//these are our output pins
#define LED1_PIN 6
#define LED2_PIN 5
#define BUZZER_PIN 3
#define SLIDER1_LED_PIN 11
#define SLIDER2_LED_PIN 10
#define SLIDER3_LED_PIN 9

//these are for the shift register
#define LATCH_PIN 8
#define CLOCK_PIN 12
#define DATA_PIN 13

//our init and bootup function
void ds_init()
{
  //buttons are inputs.
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  pinMode(BUTTON3_PIN, INPUT);
  
  //leds and buzzers are outputs.
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SLIDER1_LED_PIN, OUTPUT);
  pinMode(SLIDER2_LED_PIN, OUTPUT);
  pinMode(SLIDER3_LED_PIN, OUTPUT);

  //shift register pins are output.
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  
  //flash our LEDs
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);
  digitalWrite(SLIDER1_LED_PIN, HIGH);
  digitalWrite(SLIDER2_LED_PIN, HIGH);
  digitalWrite(SLIDER3_LED_PIN, HIGH);

  //turn 7-segment on
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 255);
  digitalWrite(LATCH_PIN, HIGH);

  //buzz for a bit.
  for (int i=0; i<500; i++)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(1915);
    digitalWrite(BUZZER_PIN, LOW);
  }

  //turn off LEDs
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(SLIDER1_LED_PIN, LOW);
  digitalWrite(SLIDER2_LED_PIN, LOW);
  digitalWrite(SLIDER3_LED_PIN, LOW);

  //turn 7-segment off
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0);
  digitalWrite(LATCH_PIN, HIGH);

  //startup serial
  Serial.begin(9600);
  Serial.println("started");
}
// Danger Board v1.0 Piezo Example
// By: Zach Hoeken (hoeken@gmail.com)
// More info: make.nycresistor.com/ds-1.0
// Code Example borrowed from http://www.arduino.cc/en/Tutorial/PlayMelody

/* Play Melody
 * -----------
 *
 * Program to play melodies stored in an array, it requires to know
 * about timing issues and about how to play tones.
 *
 * The calculation of the tones is made following the mathematical
 * operation:
 *
 *       timeHigh = 1/(2 * toneFrequency) = period / 2
 *
 * where the different tones are described as in the table:
 *
 * note 	frequency 	period 	PW (timeHigh)	
 * c 	        261 Hz 	        3830 	1915 	
 * d 	        294 Hz 	        3400 	1700 	
 * e 	        329 Hz 	        3038 	1519 	
 * f 	        349 Hz 	        2864 	1432 	
 * g 	        392 Hz 	        2550 	1275 	
 * a 	        440 Hz 	        2272 	1136 	
 * b 	        493 Hz 	        2028	1014	
 * C	        523 Hz	        1912 	956
 *
 * (cleft) 2005 D. Cuartielles for K3
 */

//our setup stuff


void setup()
{
  ds_init();
}

int ledPin = LED1_PIN; 
int speakerOut = BUZZER_PIN;
byte names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};
int tones[] = {1915, 1700, 1519, 1432, 1275, 1136, 1014, 956};
byte melody[] = "2d2a1f2c2d2a2d2c2f2d2a2c2d2a1f2c2d2a2a2g2p8p8p8p"; 
// count length: 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
//                                10                  20                  30 
int count = 0; 
int count2 = 0; 
int count3 = 0;
int MAX_COUNT = 24; 
int statePin = LOW;

void loop()
{
  //start our program
  digitalWrite(speakerOut, LOW);     
  for (count = 0; count < MAX_COUNT; count++)
  {
	//show our status
    statePin = !statePin;
    digitalWrite(ledPin, statePin);

    //loop through all the notes
    for (count3 = 0; count3 <= (melody[count*2] - 48) * 30; count3++)
    {
      for (count2=0;count2<8;count2++)
      {
	    //play our tone!
        if (names[count2] == melody[count*2 + 1])
        {       
          digitalWrite(speakerOut,HIGH);
          delayMicroseconds(tones[count2]);
          digitalWrite(speakerOut, LOW);
          delayMicroseconds(tones[count2]);
        } 

        //do we want a pause?
        if (melody[count*2 + 1] == 'p')
        {
          // make a pause of a certain size
          digitalWrite(speakerOut, 0);
          delayMicroseconds(500);
        }
      }
    }
  }
} 
