//Project 14 - Light Sensor

int ledPin = 6;  // Pin we will connect to LED         
int ldrPin = 3;  // Pin connected to LDR           
int lightVal = 0;  // Value read from LDR

void setup()          
{
 pinMode(ledPin, OUTPUT); // Set both pins as outputs
}

void loop()               
{
 lightVal = analogRead(ldrPin); // Read in value from LDR
 digitalWrite(ledPin, HIGH);  // Turn LED on
 delay(lightVal);   // Delay of length lightVal
 digitalWrite(ledPin, LOW); // Turn LED off
 delay(lightVal);  // Delay again
}
