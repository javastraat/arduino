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

void ds_init()
{
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  pinMode(BUTTON3_PIN, INPUT);
  
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SLIDER1_LED_PIN, OUTPUT);
  pinMode(SLIDER2_LED_PIN, OUTPUT);
  pinMode(SLIDER3_LED_PIN, OUTPUT);

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
  for (byte i=0; i<255; i++)
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
}
