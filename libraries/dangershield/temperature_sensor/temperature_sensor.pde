// Danger Board v1.0 Temperature Sensor
// By: Zach Hoeken (hoeken@gmail.com)
// More info: make.nycresistor.com/ds-1.0

//our setup stuff
#include "_init.h"

void setup()
{
  ds_init();
}

//our variables for temp sensing.
int raw = 0;
float voltage = 0.0;
int celsius = 0;
int fahrenheit = 0;

void loop()
{
  //take an analog reading
  raw = analogRead(TEMP_SENSOR_PIN);

  //convert it to voltage.  analogRead returns a value of 0-1023 representing the voltage level between 0.0v and 5.0v
  voltage = (raw * 5.0) / 1024.0;

  //finally, convert voltage to temperature.  the LM35 outputs 10.0mV per degree Celsius.  For example 1000mV (1v) would be 100C.
  celsius = voltage * 100;

  //of course you can combine this into one compact statement
  celsius = (5.0 * analogRead(TEMP_SENSOR_PIN) * 100.0) / 1024.0;

  //how about fahrenheit?
  fahrenheit = (celsius * 1.8) + 32;

  //now send it to the output so you can see it.
  Serial.print("Raw: ");
  Serial.println(raw, DEC);
  Serial.print("Voltage: ");
  Serial.println(voltage, DEC);
  Serial.print("Celsius: ");
  Serial.println(celsius, DEC);
  Serial.print("Fahrenheit: ");
  Serial.println(fahrenheit, DEC);
  Serial.println(" ");

  //only read temperature once a second.
  delay(1000);
}
