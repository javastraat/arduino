/* Sensor test sketch
  for more information see http://www.ladyada.net/make/logshield/lighttemp.html
  */
 
#define aref_voltage 3.3         // we tie 3.3V to ARef and measure it with a multimeter!
 
 
 
 
//TMP36 Pin Variables
int tempPin = 1;        //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures
int tempReading;        // the analog reading from the sensor
 
void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);   
 
  // If you want to set the aref to something other than 5v
  analogReference(EXTERNAL);
}
 
 
void loop(void) {
 
  tempReading = analogRead(tempPin);  
 
  Serial.println("");
  Serial.print("Temp reading = ");
  Serial.print(tempReading);     // the raw analog reading
 
  // converting that reading to voltage, which is based off the reference voltage
  float voltage = tempReading * aref_voltage;
  voltage /= 1024.0; 
 
  // print out the voltage
  Serial.print(" - ");
  Serial.print(voltage); Serial.println(" volts");
 
  // now print out the temperature
  float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((volatge - 500mV) times 100)
  Serial.print(temperatureC); Serial.println(" degress C");
 
  // now convert to Fahrenheight
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  Serial.print(temperatureF); Serial.println(" degress F");
 Serial.println("");
  delay(1000);
}
 
