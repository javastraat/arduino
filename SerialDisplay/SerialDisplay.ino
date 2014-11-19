#include <SoftwareSerial.h>
#include <DS1302.h>

// Init the DS1302
DS1302 rtc(2, 3, 4);

//TMP36 Pin Variables
int sensorPin = A5; //the analog pin the TMP36's Vout (sense) pin is connected to
//the resolution is 10 mV / degree centigrade with a
//500 mV offset to allow for negative temperatures
/*
* setup() - this function runs once when you turn your Arduino on
* We initialize the serial connection with the computer
*/
#define tx_PIN 6
SoftwareSerial LCD = SoftwareSerial(0, tx_PIN);

const int LCDdelay = 10; // conservative, 2 actually works

void clearLCD() {
  LCD.write(0xFE);   // command flag
  LCD.write(0x58);   // clear command.
  delay(LCDdelay);
}

void cursorOff() {
  LCD.write(0xFE);
  LCD.write(0x54);
  delay(LCDdelay);
}

void setup() {
  pinMode(tx_PIN, OUTPUT);
  LCD.begin(9600);
  // Set the clock to run-mode, and disable the write protection
  rtc.halt(false);
  rtc.writeProtect(false);

  // Setup Serial connection
  Serial.begin(9600);

  // The following lines can be commented out to use the values already stored in the DS1302
  // rtc.setDOW(MONDAY);        // Set Day-of-Week to FRIDAY
  // rtc.setTime(18, 30, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(10, 11, 2014);   // Set the date to August 6th, 2010
  delay(1000);        // the LCD display take some time to initialize
  clearLCD();
  cursorOff();
  LCD.print("Starting sensor");
}


void loop()
{
  //getting the voltage reading from the temperature sensor
  int reading = analogRead(sensorPin);
  // converting that reading to voltage, for 3.3v arduino use 3.3
  float voltage = reading * 5.0;
  voltage /= 1024.0;
  // print out the voltage
  Serial.print(voltage); Serial.println(" volts");
  // now print out the temperature
  float temperatureC = (voltage - 0.5) * 100 ; //converting from 10 mv per degree wit 500 mV offset
  //to degrees ((voltage - 500mV) times 100)
  Serial.print(temperatureC); Serial.println(" degrees C");
  // now convert to Fahrenheit
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  Serial.print(temperatureF); Serial.println(" degrees F");
  delay(1000); //waiting a second
  // Send Day-of-Week
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");

  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // Send time
  Serial.println(rtc.getTimeStr());

  // Wait one second before repeating :)
  //delay (1000);

 
        char buf[128];
        //   case DHT_ERROR_NONE:
        sprintf(buf, "    Temp: %hi.%01hiC\nHumidity: %i.%01i%%",
                rtc.getTimeStr(), rtc.getDOWStr());
        LCD.print(buf);
        // LCD.print("HOI");
        /*  break;
        case DHT_ERROR_CHECKSUM:
          LCD.print("Check sum error");
          break;
        case DHT_BUS_HUNG:
          LCD.print("BUS Hung");
          break;
        case DHT_ERROR_NOT_PRESENT:
          LCD.print("Not Present");
          break;
        case DHT_ERROR_ACK_TOO_LONG:
          LCD.print("ACK time out");
          break;
        case DHT_ERROR_SYNC_TIMEOUT:
          LCD.print("Sync Timeout");
          break;
        case DHT_ERROR_DATA_TIMEOUT:
          LCD.print("Data Timeout");
          break;
        case DHT_ERROR_TOOQUICK:
          LCD.print("Polled to quick");
          break;
          */
    }
