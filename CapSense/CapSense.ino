#include <CapSense.h>

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */


CapSense   cs_9_2 = CapSense(9,2);   //Initializes CapSense pins
#define LED  5
#define LED1  6
#define BUZZER   3

#define BUTTON1  10
#define BUTTON2  11
#define BUTTON3  12

int sensor=500;
int sensor1=750;

void setup()                    
{

   cs_9_2.set_CS_AutocaL_Millis(0xFFFFFFFF); // Calibrates CapSense pin timing
 
 Serial.begin(9600);
 pinMode(LED, OUTPUT);
 pinMode(LED1, OUTPUT);
 

}

void loop()                    
{
    long start = millis();
    long total1 =  cs_9_2.capSense(30);
    
    Serial.print(millis() - start);        // check on performance in milliseconds
    Serial.print("\t");                    // tab character for debug windown spacing

    Serial.print(total1);                  // print sensor output 1
    Serial.println("\t");
    
    if (total1 >= sensor)
    digitalWrite(LED,HIGH);
    
    if (total1 < sensor)
    digitalWrite(LED,LOW);
    
    if (total1 >= sensor1)
    digitalWrite(LED1,HIGH);
    
    if (total1 < sensor1)
    digitalWrite(LED1,LOW);
    
    delay(250);                             // arbitrary delay to limit data to serial port 
}
