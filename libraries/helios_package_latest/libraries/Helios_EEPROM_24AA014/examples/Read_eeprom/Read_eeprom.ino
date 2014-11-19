#include <Wire.h>

#include <Helios_EEPROM_24AA014.h>

Helios_EEPROM_24AA014 ee;


char Temp[50];

void setup()
{
  Wire.begin();
  Serial.begin(9600);
}

void loop()
{
  for(unsigned int byt = 0; byt <16;byt++){
    sprintf(Temp,"byte %d = %d",byt,ee.ByteRead(byt));
    Serial.println(Temp);
    delay(1000);
  }
  for(unsigned int integ = 0; integ <8;integ++){
    sprintf(Temp,"integer %d = %d",integ,ee.IntegerRead(integ));
    Serial.println(Temp);
    delay(1000);
  }
  for(unsigned int str =0;str <3;str++){
    sprintf(Temp,"string %d = %s",str,ee.StringRead(str));
    Serial.println(Temp);
    delay(1000);
  }
}


