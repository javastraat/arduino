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

#include "Helios_Temperature_Sensor_TMP006.h"
#include "Arduino.h"
#include "Wire.h"

#define TMPAdress 0x40

//declaraties voor object temp berekening
#define ValueS0 5E-14
#define ValueA1 0.00175
#define ValueA2 -0.00001678
#define ValueTREF 298.15
#define ValueB0 -294E-7
#define ValueB1 -57E-8
#define ValueB2 463E-11
#define ValueC2 13.4

float Tobject;
float Tambient;

float Helios_Temperature_Sensor_TMP006::ReadAmbient()
{
	Helios_Temperature_Sensor_TMP006::WireConfig();
	Tambient = (Helios_Temperature_Sensor_TMP006::GetAmbTemp())*(0.0078512);
	return Tambient;
}

float Helios_Temperature_Sensor_TMP006::ReadObject()
{
	Helios_Temperature_Sensor_TMP006::WireConfig();
	Tobject = Helios_Temperature_Sensor_TMP006::CalcObjTemp();
	return Tobject;
}

  void Helios_Temperature_Sensor_TMP006::WireConfig(){ 
  
  Wire.beginTransmission(TMPAdress);
  Wire.write(0x02); //write to congig register
  Wire.write(0x71); //config register[15-8]
  Wire.write(0x00); //config register[7-0]
  Wire.endTransmission();
  delay(15);
}


float Helios_Temperature_Sensor_TMP006::GetObjTemp()
{
  int xh, xl,temp;
  float result;  
  
  Wire.beginTransmission(TMPAdress); //sensor adress
  Wire.write(0x00); //adress object temp
  Wire.endTransmission();
  delayMicroseconds(2); //min 1,3us delay
  
  Wire.requestFrom(TMPAdress,2);
  while(Wire.available()){
    xh = Wire.read();
    xl = Wire.read();}
    temp = (xh<<8)+xl;
    if(temp>0x7FFF) temp=temp-0x10000;
    result=(float)temp;
    return result;
}

float Helios_Temperature_Sensor_TMP006::GetAmbTemp()
{
  unsigned int ambient, yh,yl;
  float result;
  
  Wire.beginTransmission(TMPAdress); //sensor adress
  Wire.write(0x01);  //adress ambient temp
  Wire.endTransmission();
  delayMicroseconds(2); //min 1,3usec delay
  
  Wire.requestFrom(TMPAdress,2);
  while(Wire.available()){
    yh = Wire.read();
    yl = Wire.read();
  }
  ambient = (yl + (yh <<8));
  result = (float)ambient;
  return result;   
}

//bereken uit de ingelezen register de object en ambient temperatuur
float Helios_Temperature_Sensor_TMP006::CalcObjTemp()
{
  float Vobject=(Helios_Temperature_Sensor_TMP006::GetObjTemp())  *(0.00000015625);
  float tambient=(Helios_Temperature_Sensor_TMP006::GetAmbTemp())*(0.0078512)+273.15;
  float S = ValueS0*(1+ (ValueA1*(tambient-ValueTREF)) + (ValueA2*((tambient-ValueTREF)*(tambient-ValueTREF))));
  float Vos = (ValueB0 + (ValueB1*(tambient-ValueTREF))+(ValueB2*((tambient-ValueTREF)*(tambient-ValueTREF))));
  float F = (Vobject-Vos)+ (ValueC2*((Vobject-Vos)*(Vobject-Vos)));
  float tobject = sqrt(sqrt( pow(tambient,4) + (F/S)));
  tobject = tobject - 273.15;
  //tambient = tambient - 273.15;
  return tobject;
}

