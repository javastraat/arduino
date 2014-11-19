#include <LiquidCrystal.h>


// LiquidCrystal display with:
// rs on pin 12
// rw on pin 11
// enable on pin 10
// d4, d5, d6, d7 on pins 5, 4, 3, 2
LiquidCrystal lcd(8, 7, 6, 5, 4, 3, 2);
int ledPin = 9;		    // LED connected to digital pin 13
int recvPin=0;
int wait=5000;
int wait1=0;
int val=0;

byte logoc [8] = {
  B11000,
  B11000,
  B00000,
  B00111,
  B00100,
  B00100,
  B00100,
  B00111
};
byte logof [8] = {
  B11000,
  B11000,
  B00000,
  B00111,
  B00100,
  B00110,
  B00100,
  B00100
};
byte ard1 [8] = {
  B00111,
  B01100,
  B11000,
  B10011,
  B10011,
  B11000,
  B01100,
  B00111
};
byte ard2 [8] = {
  B11100,
  B00110,
  B00011,
  B11001,
  B11001,
  B00011,
  B00110,
  B11100
};
byte ard3 [8] = {
  B00111,
  B01100,
  B11001,
  B10011,
  B10011,
  B11001,
  B01100,
  B00111
};
byte ard4 [8] = {
  B11100,
  B00110,
  B10011,
  B11001,
  B11001,
  B10011,
  B00110,
  B11100
};
byte smiley [8] = {
  B00000,
  B00000,
  B01010,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000
};


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

  lcd.createChar(0, logoc);
  lcd.createChar(1, logof);
  lcd.createChar(2, ard1);
  lcd.createChar(3, ard2);
  lcd.createChar(4, ard3);
  lcd.createChar(5, ard4);
  lcd.createChar(6, smiley);

  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);   
  lcd.begin(16,2);
  // If you want to set the aref to something other than 5v
  analogReference(EXTERNAL);

  lcd.setCursor(0, 0);
  lcd.write(2);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
  lcd.print(" * Arduino *");
  lcd.setCursor(0, 1);
  lcd.print(":) ");
  lcd.write(6);
  lcd.print(" * booting * ");
  delay (wait);

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
  Serial.print(voltage); 
  Serial.println(" volts");

  // now print out the temperature
  float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
  //to degrees ((volatge - 500mV) times 100)
  Serial.print(temperatureC); 
  Serial.println(" degress C");

  // now convert to Fahrenheight
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  Serial.print(temperatureF); 
  Serial.println(" degress F");
  Serial.println("");
  delay(1000);

  //write results to lcd
  lcd.setCursor(0, 0);
  lcd.write(2);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
  lcd.print(" ");
  lcd.print(voltage);
  lcd.print("V*0");
  lcd.print(tempReading);
  lcd.print("T*");

  lcd.setCursor(0, 1);
  lcd.print("*");
  lcd.print(temperatureC);
  lcd.write(0);
  lcd.print("**");
  lcd.print(temperatureF);
  lcd.write(1);
  lcd.print("*");
  // delay (wait);
  //lcd.clear();
  //delay (wait1);
}







