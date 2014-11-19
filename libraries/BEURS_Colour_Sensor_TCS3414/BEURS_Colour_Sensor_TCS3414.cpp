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

#include "BEURS_Colour_Sensor_TCS3414.h"
#include "Arduino.h"
#include "Wire.h"
#include <Helios_EEPROM_24AA014.h>


Helios_EEPROM_24AA014 eee;

int Delaytime;
int Stand = 4;

//default sensor settings
unsigned int Time = 2; //Intergration time is 400ms
unsigned int Gain = 1; //64X versterken
unsigned int Presc = 1; //geen prescaler
unsigned int IntTime = 400;

unsigned int Green;
unsigned int Blue;
unsigned int Red;
unsigned int Clear;

boolean UpdateConfig = true;

void BEURS_Colour_Sensor_TCS3414::Init()
{
  byte intergrationtime, gainvalue,prescvalue;
  
  if(Time == 0){
    Delaytime = 12+10;
    intergrationtime = 0x00; //12ms
	//intergrationtime = 0x02; //400ms
	IntTime = 12;
  }
  if(Time == 1){
	Delaytime = 100+10;
    intergrationtime = 0x01; //100ms
	IntTime = 100;
  }
  if(Time == 2){
	Delaytime = 400+10;
    intergrationtime = 0x02; //400ms
	//intergrationtime = 0x00; //400ms
	IntTime = 400;
  }
  if(Stand == 1){
	gainvalue = 3;
	prescvalue = 0x00;
	Gain = 64;
	Presc = 1;
  }
  if(Stand == 2){
	gainvalue = 2;
	prescvalue = 0x00;
	Gain = 16;
	Presc = 1;
  }
  if(Stand == 3){
	gainvalue = 1;
	prescvalue = 0x00;
	Gain = 4;
	Presc = 1;
  }
  if(Stand == 4){
	gainvalue = 0;
	prescvalue = 0x00;
	Gain = 1;
	Presc = 1;
  }
  if(Stand == 5){
	gainvalue = 0x00;
	prescvalue = 0x02;
	Gain = 1;
	Presc = 4;
  }
  if(Stand == 6){
	gainvalue = 0x00;
	prescvalue = 0x04;
	Gain = 1;
	Presc = 16;
  }
  if(Stand == 7){
	gainvalue = 0x00;
	prescvalue = 0x06;
	Gain = 1;
	Presc = 64;
  }
  
  byte gainpresc = (gainvalue<<4) + prescvalue;
  
  Wire.beginTransmission(0x39);
  Wire.write(0x80);  //cmd mode, control reg
  Wire.write(0x01); // power on
  Wire.endTransmission();
  
  Wire.beginTransmission(0x39);
  Wire.write(0x81); //cmd mode, time reg
  Wire.write(intergrationtime); //intergration time 
  Wire.endTransmission();
  
  Wire.beginTransmission(0x39);
  Wire.write(0x87); //cmd mode, gain reg
  Wire.write(gainpresc); //gain/pres 
  Wire.endTransmission();
  
  Wire.beginTransmission(0x39);
  Wire.write(0x80); //cmd mode, control reg
  Wire.write(0x03); // power on, adc enable
  Wire.endTransmission();
  
  delay(Delaytime); 
}

void BEURS_Colour_Sensor_TCS3414::Readlow(int t,int ga, int p,unsigned int* r,unsigned int* g,unsigned int* b/*,unsigned int* c,unsigned int* l, unsigned int* ct*/)
{
  Gain = ga;
  Presc = p;
  unsigned int millis1 = millis();
  byte intergrationtime, gainvalue,prescvalue;
  if(t == 12){
    Delaytime = 12+10;
    intergrationtime = 0x00; //12ms
	IntTime = 12;
  }
  if(t == 100){
	Delaytime = 100+10;
    intergrationtime = 0x01; //100ms
	IntTime = 100;
  }
  if(t == 400){
	Delaytime = 400+10;
    intergrationtime = 0x02; //400ms
	IntTime = 400;
  } 
  
  if(ga == 1){
    gainvalue = 0;
  }  
  if(ga == 4){
    gainvalue = 1;
  } 
  if(ga == 16){
    gainvalue = 2;
  } 
  if(ga == 64){
    gainvalue = 3;
  }
  if(p == 1){
   prescvalue = 0x00;
  }
  if(p == 2){
   prescvalue = 0x01;
  }
  if(p == 4){
   prescvalue = 0x02;
  }
  if(p == 8){
   prescvalue = 0x03;
  }
  if(p == 16){
   prescvalue = 0x04;
  }
  if(p == 32){
   prescvalue = 0x05;
  }
  if(p == 64){
   prescvalue = 0x06;
  }
  
  byte gainpresc = (gainvalue<<4) + prescvalue;
  
  Wire.beginTransmission(0x39);
  Wire.write(0x80);  //cmd mode, control reg
  Wire.write(0x01); // power on
  Wire.endTransmission();
  
  Wire.beginTransmission(0x39);
  Wire.write(0x81); //cmd mode, time reg
  Wire.write(intergrationtime); //intergration time 
  Wire.endTransmission();
  
  Wire.beginTransmission(0x39);
  Wire.write(0x87); //cmd mode, gain reg
  Wire.write(gainpresc); //gain/pres 
  Wire.endTransmission();
  
  Wire.beginTransmission(0x39);
  Wire.write(0x80); //cmd mode, control reg
  Wire.write(0x03); // power on, adc enable
  Wire.endTransmission();
  
  delay(Delaytime);
  
  Green = BEURS_Colour_Sensor_TCS3414::TcsGetGreen();
  Red = BEURS_Colour_Sensor_TCS3414::TcsGetRed();
  Blue = BEURS_Colour_Sensor_TCS3414::TcsGetBlue();
  Clear = BEURS_Colour_Sensor_TCS3414::TcsGetClear();
  
  /*float Redn = Red;
  Redn=Red/(float)Gain;
  Redn=Redn*(float)Presc;
  Redn = Redn*(float)(IntTime/12.0);
  float Greenn = Green;
  Greenn = Green/(float)Gain;
  Greenn = Greenn*(float)Presc;
  Greenn = Greenn*(float)(IntTime/12.0);
  float Bluen = Blue;
  Bluen = Blue/(float)Gain;
  Bluen = Bluen*(float)Presc;
  Bluen = Bluen*(float)(IntTime/12.0);
  
  float TristX = (-0.14282)*(Redn)+(1.54924)*(Greenn)+(-0.95641)*(Bluen);
  float TristY = (-0.32466)*(Redn)+(1.57837)*(Greenn)+(-0.73191)*(Bluen);
  float TristZ = (-0.68202)*(Redn)+(0.77073)*(Greenn)+(0.56322)*(Bluen);
  
  float XYZ = TristX + TristY + TristZ;
  float x = TristX / XYZ;
  float y = TristY / XYZ;
  
  float n  = (x - 0.3320)/(0.1858 - y);
  int CCT = (449*n*n*n) + (3525*n*n) + 6823.3*n + 5520.33;
  
  TristY = TristY/100;*/
  
  *r = Red;
  *g = Green;
  *b = Blue;
  /*c = Clear;
  *l = TristY;
  *ct = CCT;*/
  
  unsigned int millis2 = millis();
  unsigned int progtime = millis2 - millis1;
  delay(500-progtime);  //meet 2 keer per seconde 
 }

void BEURS_Colour_Sensor_TCS3414::Read(/*unsigned int* red,unsigned int* green,unsigned int* blue,unsigned int* clear, unsigned int* t, unsigned int* g, unsigned int* p, */unsigned int* l, unsigned int* ct)
{
  
  unsigned int millis1 = millis();
  BEURS_Colour_Sensor_TCS3414::Init();
  float fgain=(float)Gain;
  float fpresc=(float)Presc;
  float ftime = (float)(400/IntTime);
  unsigned int factorred =  eee.IntegerRead(0);
  unsigned int factorgreen = eee.IntegerRead(1);
  unsigned int factorblue = eee.IntegerRead(2);
  bool result = true;
  //if(800>=factorred<=1200 && 800>=factorgreen<=1200 && 800>=factorblue<=1200){}
  if(factorred < 800) result = false;
  if(factorred >1200) result = false;
  if(factorgreen < 800) result = false;
  if(factorgreen >1200) result = false;
  if(factorblue < 800) result = false;
  if(factorblue >1200) result = false;
  if(!result){
	factorred = 1000;
	factorgreen = 1000;
	factorblue = 1000;
 }
	
  //Green = LichtSensorClass::TcsGetGreen() * (12000/eee.IntegerRead(1));
  //Red = LichtSensorClass::TcsGetRed() * (14000/eee.IntegerRead(2));
  //Blue = LichtSensorClass::TcsGetBlue() * (16000/eee.IntegerRead(3));
  Green = BEURS_Colour_Sensor_TCS3414::TcsGetGreen();
  Red = BEURS_Colour_Sensor_TCS3414::TcsGetRed();
  Blue = BEURS_Colour_Sensor_TCS3414::TcsGetBlue();
  Clear = BEURS_Colour_Sensor_TCS3414::TcsGetClear();

  float Redn = Red;
  //Redn = Red * 1000.0/((float)eee.IntegerRead(0));
  Redn = Red * 1000.0/((float)factorred);
  Redn=Redn/fgain;
  Redn=Redn*fpresc;
  Redn = Redn*ftime;
  float Greenn = Green;
  //Greenn = Green * 1000.0/((float)eee.IntegerRead(1));
  Greenn = Green * 1000.0/((float)factorgreen);
  Greenn = Greenn/fgain;
  Greenn = Greenn*fpresc;
  Greenn = Greenn*ftime;
  float Bluen = Blue;
  //Bluen = Blue * 1000.0/((float)eee.IntegerRead(2));
  Bluen = Blue * 1000.0/((float)factorblue);
  Bluen = Bluen/fgain;
  Bluen = Bluen*fpresc;
  Bluen = Bluen*ftime;
  
  float TristX = (-0.0905)*(Redn)+(0.5182)*(Greenn)+(-0.3935)*(Bluen);
  float TristY = (-0.1143)*(Redn)+(0.5503)*(Greenn)+(-0.4079)*(Bluen);
  float TristZ = (-0.1734)*(Redn)+(0.3814)*(Greenn)+(-0.1295)*(Bluen);
  
  float XYZ = TristX + TristY + TristZ;
  float x = TristX / XYZ;
  float y = TristY / XYZ;
  
  float n  = (x - 0.3320)/(0.1858 - y);
  unsigned int CCT = (449*n*n*n) + (3525*n*n) + 6823.3*n + 5520.33;
  
  /**red = Red;
  *green = Green;
  *blue = Blue;
  *clear = Clear;
  *t = IntTime;
  *g = Gain;
  *p = Presc;*/
  *l = TristY;
  *ct = CCT;

  
  BEURS_Colour_Sensor_TCS3414::AutoSet();
  unsigned int millis2 = millis();
  unsigned int progtime = millis2 - millis1;
  delay(500-progtime);  //meet 2 keer per seconde
}

unsigned int BEURS_Colour_Sensor_TCS3414::TcsGetGreen()
{
  unsigned int GreenHigh;
  unsigned int GreenLow;
  Wire.beginTransmission(0x39);
  //Wire.write(0x90); //cmd, mode, greenlow reg, byte mode
  Wire.write(0xB0);
  Wire.endTransmission();
  
  Wire.beginTransmission(0x39);
  Wire.requestFrom(0x39,2);
  GreenLow = Wire.read();
  GreenHigh = Wire.read();
  Wire.endTransmission();
  
  GreenHigh = (GreenHigh * 256) + GreenLow;
  return GreenHigh;
}

unsigned int BEURS_Colour_Sensor_TCS3414::TcsGetRed()
{
  unsigned int RedHigh;
  unsigned int RedLow;
  Wire.beginTransmission(0x39);
  //Wire.write(0x92); //cmd mode,redlow reg, byte mode
  Wire.write(0xB2);
  Wire.endTransmission();
  
  Wire.beginTransmission(0x39);
  Wire.requestFrom(0x39,2);
  RedLow = Wire.read();
  RedHigh = Wire.read();
  Wire.endTransmission();
  
  RedHigh = (RedHigh * 256) + RedLow;
  return RedHigh;
}
  
unsigned int BEURS_Colour_Sensor_TCS3414::TcsGetBlue()
{
  unsigned int BlueHigh;
  unsigned int BlueLow;
  Wire.beginTransmission(0x39);
  //Wire.write(0x94); //cmd mode, bluelow reg, byte mode
  Wire.write(0xB4);
  Wire.endTransmission();
  
  Wire.beginTransmission(0x39);
  Wire.requestFrom(0x39,2);
  BlueLow = Wire.read();
  BlueHigh = Wire.read();
  Wire.endTransmission();
  
  BlueHigh = (BlueHigh * 256) + BlueLow;
  return BlueHigh;
}

unsigned int BEURS_Colour_Sensor_TCS3414::TcsGetClear()
{
  unsigned int ClearHigh;
  unsigned int ClearLow;
  Wire.beginTransmission(0x39);
  //Wire.write(0x96); //cmd mode, clearlow reg, byte mode
  Wire.write(0xB6);
  Wire.endTransmission();
  
  Wire.beginTransmission(0x39);
  Wire.requestFrom(0x39,2);
  ClearLow = Wire.read();
  ClearHigh = Wire.read();
  Wire.endTransmission();
  
  ClearHigh = (ClearHigh * 256) + ClearLow;
  return ClearHigh;
}

void BEURS_Colour_Sensor_TCS3414::AutoSet()
{
	UpdateConfig = true;
	
	if(UpdateConfig){
		if(Time == 0){
			if(Clear < 1000 && Clear > 200){
				UpdateConfig = false;
				Time++;
			}
			if(Clear <= 200){
				UpdateConfig = false;
				Time = 2;
			}
		}		
	}
	
	if(UpdateConfig){
		if(Time == 1){
			if(Clear > 9999){
				UpdateConfig = false;
				Time--;
			}
		}
	}
	
	if(UpdateConfig){
		if(Time == 1){
			if(Clear < 1000){
				UpdateConfig = false;
				Time++;
			}
		}
	}
	
	if(UpdateConfig){
		if(Time == 2){
			if(Clear < 1000){
				UpdateConfig = false;
				if(Clear <= 10){
					Stand = Stand - 4;
				}
				if(Clear <= 50){
					Stand = Stand - 3; 
				}
				if(Clear < 200){
					Stand = Stand - 2;
				}
				if(Clear >= 200){
					Stand = Stand - 1;
				}
				if(Stand <= 0){ //gevoeligste instellingen bereikt
					Stand = 1;
				}
			}
		}
	}
	
	if(UpdateConfig){
		if(Time == 2){
			if(Clear > 10000){  //saturation at 400ms&gain 64x = 26000
				if(Stand == 7){
					if(Clear >= 38000){
						Time = 0;
					}
					if(Clear < 38000){
						Time = 1;
					}
				}
				if(Clear >= 50000){
					Stand = Stand + 3;
				}
				if(Clear >= 18000 && Clear < 50000){
					Stand = Stand + 2;
				}
				if(Clear < 18000){
					Stand++;
				}
				if(Stand >= 8){
					Stand = 7; //minst gevoelige instellingen bereikt;
				}
			}
		}
	}
}
				
	
	
	



/*void LichtSensorClass::AutoSet()
{
	UpdateConfig = true;
	
	if(UpdateConfig){
		if(Time == 0){
			if(Clear < 1000 ){
				UpdateConfig = false;
				Time++;
			}
		}		
	}
	
	if(UpdateConfig){
		if(Time == 1){
			if(Clear > 9999){
				UpdateConfig = false;
				Time--;
			}
		}
	}
	
	if(UpdateConfig){
		if(Time == 1){
			if(Clear < 1000){
				UpdateConfig = false;
				Time++;
			}
		}
	}
	
	if(UpdateConfig){ //first: decrease presc
		if(Time == 2){
			if(Clear < 1000){
				if(Presc <= 7 && Presc >1){
					UpdateConfig = false;
					Presc--;
				}
			}
		}
	}
	
	if(UpdateConfig){//second: increase gain
		if(Time == 2){
			if(Clear < 1000 ){
				if(Gain < 4){
					UpdateConfig = false;
					Gain++;
				}
			}
		}
	}
	
	
	if(UpdateConfig){ //first: decrease gain
		if(Time == 2){
			if(Clear > 5000 ){
				if(Gain <= 4 && Gain > 1){
					UpdateConfig = false;
					Gain--;
				}
			}
		}
	}
	
	if(UpdateConfig){ // second: increase presc
		if(Time == 2){
			if(Clear > 5000 ){
				if(Presc < 7){
					UpdateConfig = false;
					Presc++;
				}
				else{ //thirth: decrease intergration time
					UpdateConfig = false;
					Time--;
				}
			}
		}
	}
	

}*/

/*void LichtSensorClass::AutoSet()
{
	UpdateConfig = true;
	if(UpdateConfig){
		if(Time == 0){
			if(Clear > MaxAdcValue){
				UpdateConfig = false;
				if(Presc == 7){}
				else{
				Presc++;
				}
			}
		}
	}
	if(UpdateConfig){
		if(Time == 0){
			if(Clear < MinAdcValue){
				UpdateConfig = false;
				if(Presc <= 7){
				Presc--;
				}
				if(Presc == 0){
					Presc = 1;
					Time++;
				}
			}		
		}
	}
	if(UpdateConfig){
		if(Time == 1){
			if(Clear > MaxAdcValue){
				UpdateConfig = false;
				Time--;
			}
		}
	}
	if(UpdateConfig){
		if(Time == 1){
			if(Clear < MinAdcValue){
				UpdateConfig = false;
				Time++;
			}
		}
	}
	if(UpdateConfig){
		if(Time ==2){
			if(Clear < MinAdcValue){
				UpdateConfig = false;
				if(Gain == 4){}
				else{
					Gain++;
				}
			}
		}
	}
	if(UpdateConfig){
		if(Time == 2){
			if(Clear > DecreaseGainValue){
				UpdateConfig = false;
				if(Gain <= 4){
					Gain--;
				}
				if(Gain == 0){
					Gain = 1;
					Time--;
				}
			}
		}  
	}
}*/


