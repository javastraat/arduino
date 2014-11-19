/*
    Copyright © 2013 Mitch Oonk.
	
	"Arduino" is a trademark from the Arduino team. 
	"Helios" is a design ©2013 by KITT Engineering

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software
    is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "Helios_EEPROM_24AA014.h"
#include "Arduino.h"
#include "Wire.h"

#define EEPROMAdress B1010000

void Helios_EEPROM_24AA014::_Write(int adress, unsigned int data)
{
  unsigned int value = data;
  
  Wire.beginTransmission(EEPROMAdress);
  Wire.write(adress);
  Wire.write(value);
  Wire.endTransmission(); 
  
  delay(5);
}

unsigned int Helios_EEPROM_24AA014::_Read(int adress)
{
  unsigned int x;
  Wire.beginTransmission(EEPROMAdress);
  Wire.write(adress);
  Wire.endTransmission();
  
  Wire.requestFrom(EEPROMAdress,1);
  while(Wire.available()){
	x = Wire.read();
  }
  Wire.endTransmission();
  delay(5);
  
  return x;
 }
 
 void Helios_EEPROM_24AA014::_PageWrite(int adress, char *data)
{
  int l = strlen(data);
  if(data[strlen(data)-1]!=' ') l = 31; data[31] = NULL;
  Wire.beginTransmission(EEPROMAdress);
  Wire.write(adress);
  for(int i = 0;i<16;i++){
	Wire.write(data[i]);
  }
  Wire.endTransmission(); 
  delay(5);
  
  Wire.beginTransmission(EEPROMAdress);
  Wire.write(adress+16);
  for(int i = 16;i<=l;i++){
	Wire.write(data[i]);
  }
  Wire.endTransmission(); 
  delay(5);
}

char *Helios_EEPROM_24AA014::_PageRead(int adress)
{
static char result[32];
char c;
int i;

	i=0;
	do
	{
		c = Helios_EEPROM_24AA014::_Read(adress+i);
		result[i]=c; 
		if( c == -1){
			result[0] = 0;
			return result;
		}
		i++;
	} while(c!=0);
	return result;	
 }
 
void Helios_EEPROM_24AA014::ByteWrite(int bytenumber, int data)
 {
	if(bytenumber > 15) bytenumber = 15;
	
	if(bytenumber < 0) bytenumber = 0;
	
	if(data > 255) data = 255;
	
	Helios_EEPROM_24AA014::_Write(bytenumber,data);
	delay(5);
}

int Helios_EEPROM_24AA014::ByteRead(int bytenumber)
{
	if(bytenumber > 15) bytenumber = 15;
	
	if(bytenumber < 0) bytenumber = 0;
	
	int x = Helios_EEPROM_24AA014::_Read(bytenumber);
	return x;
}

void Helios_EEPROM_24AA014::IntegerWrite(int integernumber, unsigned int data)
{
	int adress;
	if(integernumber > 7)integernumber=7;

	if(integernumber < 0)integernumber=0;
	
	if(data > 0xFFFF) data = 0xFFFF;

	adress = 16+(integernumber*2);
	unsigned int dta[2];
	dta[0] = data  & 0xFF;
	dta[1] = (data>>8) & 0xFF;
	for(int i = 0; i<2; i++){
		Helios_EEPROM_24AA014:_Write((adress+i),dta[i]);
	}
}

unsigned int Helios_EEPROM_24AA014::IntegerRead(int integernumber)
{
	int adress;
	if(integernumber > 7)integernumber=7;

	if(integernumber < 0)integernumber=0;

	adress = 16+(integernumber*2);
	unsigned int dta[2];
	for(int i = 0; i < 2; i++)
	{
		dta[i] = Helios_EEPROM_24AA014::_Read(adress+i);
	}
	unsigned int data = dta[0] + (dta[1]<<8);
	return data;
}

int Helios_EEPROM_24AA014::StringWrite(int stringnumber, char *data)
{
	int l = strlen(data);
	int adress1, adress2;
	if(stringnumber > 2) stringnumber = 2;
	
	if(stringnumber < 0) stringnumber = 0;


	adress1 = 32 +(stringnumber*32);
	Helios_EEPROM_24AA014:_PageWrite(adress1, data);
}

char *Helios_EEPROM_24AA014::StringRead(int stringnumber)
{
	if(stringnumber > 2) stringnumber = 2;
	
	if(stringnumber < 0) stringnumber = 0;
	
	static char result[32];
	char c;
	int i;
	int adress = 32 +(stringnumber*32); 
	
	return Helios_EEPROM_24AA014::_PageRead(adress);;
}
	

		



	