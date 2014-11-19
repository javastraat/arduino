#include <CapSense.h>

/*
 * Danger Shield Example Sketch
 * Copyright (c) 2010 SparkFun Electronics.  All right reserved.
 * Written by Chris Taylor
 *
 * This code was written to demonstrate the Danger Shield from SparkFun Electronics
 * 
 * This code will test all of the devices on the Danger Shield one at a time. 
 * Press button 1 (D10) to cycle through the different tests. View their output on 
 * a terminal running at 9600 baud. 
 *
 * http://www.sparkfun.com
 */

// Shift register bit values to display 0-9 on the seven-segment display
const byte ledCharSet[10] = {

  B00111111,B00000110,B01011011,B01001111,B01100110,B01101101,B01111101,B00000111,B01111111,B01101111
};

// Global variables
int val = 0;
int state = 0;
int x = 0;
int i = 0;

// Pin definitions
#define SLIDER1  0
#define SLIDER2  1
#define SLIDER3  2

#define KNOCK    5

#define BUTTON1  10
#define BUTTON2  11
#define BUTTON3  12

#define LED1  5
#define LED2  6

#define BUZZER   3

#define TEMP  4

#define LIGHT  3

#define LATCH 7
#define CLOCK 8
#define DATA 4

// State machine values
#define SLIDER_TEST 1
#define BUZZER_TEST 2
#define CAPSENSE_TEST  3
#define TEMP_TEST  4
#define LIGHT_TEST 5
#define BUTTON_TEST 6
#define SEVENSEG_TEST 7

CapSense   cs_9_2 = CapSense(9,2);   //Initializes CapSense pins

void setup()
{
  Serial.begin(9600);
  
  cs_9_2.set_CS_AutocaL_Millis(0xFFFFFFFF); // Calibrates CapSense pin timing
  
  pinMode(BUTTON1,INPUT);
  pinMode(BUTTON2,INPUT);
  pinMode(BUTTON3,INPUT);
  
  digitalWrite(BUTTON1,HIGH);
  digitalWrite(BUTTON2,HIGH);
  digitalWrite(BUTTON3,HIGH);
  
  
  pinMode(BUZZER, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA,OUTPUT);
  
  

  Serial.println("Danger Shield Component Test");  
  Serial.println("Press Button 1 to begin.");  
}

void loop()
{
  
  
  if(!(digitalRead(BUTTON1))) // Change state
  {
    delay(1); // Debounce
    state++;
    if(state > 7){ 
      state = 1; 
    }
    while(!(digitalRead(BUTTON1)));
  }

  if(state == SLIDER_TEST) // Displays values of sliders
  {
    Serial.print("Sliders: ");
    val = analogRead(SLIDER1);
    Serial.print(" ");
    Serial.print(val);
    val = analogRead(SLIDER2);
    Serial.print("  ");
    Serial.print(val);
    val = analogRead(SLIDER3);
    Serial.print("  ");
    Serial.println(val);
    delay(300);
  }

  if(state == BUZZER_TEST) // Activates buzzer
  {
    for(int x = 0; x < 100; x++)
    {
      digitalWrite(BUZZER, HIGH);
      delay(1);
      digitalWrite(BUZZER, LOW);
      delay(1);
    }
  }

  if(state == CAPSENSE_TEST) // Tests CapSense pad
  {
    long start = millis();
    long total1 =  cs_9_2.capSense(30);
    Serial.println(total1); 
    delay(10);
  }

  if(state == TEMP_TEST) // Displays temp sensor values
  {
    val = analogRead(TEMP);
    Serial.print("Temp: ");
    Serial.println(val);
  }

  if(state == LIGHT_TEST) // Displays light sensor values
  {
    val = analogRead(LIGHT);
    Serial.print("Light: ");
    Serial.println(val);
  }

  if(state == BUTTON_TEST) // Toggles LED's depending on Buttons 2 and 3
  {  
    Serial.println("Button 2 & 3 Test");
    if(digitalRead(BUTTON3))
    {
      digitalWrite(LED1,HIGH);

    }
    else
    {
      digitalWrite(LED1,LOW);

    }
    if(digitalRead(BUTTON2))
    {
      digitalWrite(LED2,HIGH);
    }
    else
    {
      digitalWrite(LED2,LOW);
    }
  }

  if(state == SEVENSEG_TEST) // Cycles through 0-9 on seven-segment
  {
    i = 0;
    Serial.println("Seven segment display test");
    while(1)
    {
      digitalWrite(LATCH,LOW);
      shiftOut(DATA,CLOCK,MSBFIRST,~(ledCharSet[i]));
      digitalWrite(LATCH,HIGH);
      i++;
      if(i==10){
        i = 0;
      }
      delay(500);
    }
  }
}

