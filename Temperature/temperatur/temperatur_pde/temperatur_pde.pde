/*     ---------------------------------------------------------
 *     |  Arduino Experimentation Kit Example Code             |
 *     |  CIRC-10 .: Temperature :. (TMP36 Temperature Sensor) |
 *     ---------------------------------------------------------
 *   
 *  A simple program to output the current temperature to the IDE's debug window 
 * 
 *  For more details on this circuit: http://tinyurl.com/c89tvd 
 */

//TMP36 Pin Variables
int temperaturePin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
//the resolution is 10 mV / degree centigrade 
//(500 mV offset) to make negative temperatures an option

int LED1=9;
int LED2=8;
int LED3=7;
int LED4=6;
int LED5=5;
int LED6=4;
int LED7=3;
int LED8=2;
int LED9=1;


int temp=0;
/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
void setup()
{
 // Serial.begin(9600);  //Start the serial connection with the copmuter
  //to view the result open the serial monitor 
  //last button beneath the file bar (looks like a box with an antenae)

  pinMode(0, OUTPUT); 
  pinMode(1, OUTPUT); 
  pinMode(2, OUTPUT);  // specify Display outputs
  pinMode(3, OUTPUT); 
  pinMode(4, OUTPUT); 
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT); 
  pinMode(7, OUTPUT); 
  pinMode(8, OUTPUT); 
  pinMode(9, OUTPUT); 
}

void loop()                     // run over and over again
{
  float temperature = getVoltage(temperaturePin);  //getting the voltage reading from the temperature sensor
  temperature = (temperature - .5) * 100;          //converting from 10 mv per degree wit 500 mV offset
  temp = temperature;                                                  //to degrees ((volatge - 500mV) times 100)

  Serial.print(temperature);  //printing the result
  Serial.println(" Celcius");
  delay(1000);                                     //waiting a second


  if (temp > 5)
  { 
    Serial.println("1 LED");
    digitalWrite(9, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
    digitalWrite(1, LOW);

  }

  if (temp > 10)
  { 
    Serial.println("2 LED");
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
    digitalWrite(1, LOW);

  }

  if (temp > 15)
  { 
    Serial.println("3 LED");
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
    digitalWrite(1, LOW);

  }

  if (temp > 20)
  { 
    Serial.println("4 LED");
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
    digitalWrite(1, LOW);

  }

  if (temp > 22.5)
  { 
    Serial.println("5 LED");  
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
    digitalWrite(1, LOW);

  }

  if (temp > 27.5)
  { 
    Serial.println("6 LED");  
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
    digitalWrite(1, LOW);

  }

  if (temp > 30)
  { 
    Serial.println("7 LED");  
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(1, LOW);

  }

  if (temp > 31)
  { 
    Serial.println("8 LED");  
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(1, LOW);

  }

  if (temp > 32)
  { 
    Serial.println("9 LED");  
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(1, HIGH);

  }

 if (temp > 33)
  { 
    Serial.println("10 LED");  
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(0, HIGH);

  }

}

/*
 * getVoltage() - returns the voltage on the analog input defined by
 * pin
 */
float getVoltage(int pin){
  return (analogRead(pin) * .004882814); //converting from a 0 to 1024 digital range
  // to 0 to 5 volts (each 1 reading equals ~ 5 millivolts


} 

