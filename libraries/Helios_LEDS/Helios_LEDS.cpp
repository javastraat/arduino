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

#include <Helios_LEDS.h>
#include <SPI.h>
#include "Arduino.h"

#include <avr/pgmspace.h>

#define RESET_VALUE2 65285 //2ms
#define RESET_VALUE1 65410 //1ms
#define RESET_TIMER3 TCNT3 = RESET_VALUE1 //Timer3 reset value
#define RESET_TIMER1 TCNT1 = RESET_VALUE1


int LedEnable  = A2;
int LedLatch  = A3;

unsigned int Buflength;
unsigned int ResetTempPos;
unsigned int Kleur = 0;
unsigned int Ledvalue;
unsigned int Value = 0;
unsigned int LineCount = -1;

unsigned int WaardeLed;
unsigned int CheckLed;

boolean UpdateText = false;
volatile boolean Done = false;
boolean StartPos = true;
boolean Looop = false;
boolean Interv = false;
boolean Waarde1 = false;
boolean Led = false;

char AsciiValues[32];
char Temp[6*32];

unsigned int Interval = 0;
unsigned int MsCount = 0;
unsigned int TempPosition = 0;
unsigned int CountLine = 0;
unsigned int Scrollspeed = 1000/20; //default

ISR(TIMER1_OVF_vect){
  RESET_TIMER1; //reset instellingen voor overflow elke ms
  MsCount++;
  CountLine++;
  if(CountLine<5){
	Helios_LEDS::WriteMatrix(); //voor WriteMatix elke ms uit.
  }
  if(CountLine == 5){
	CountLine = -1; //begin volgende keer weer bij lijn  0
	Helios_LEDS::WriteLed();
	}
  if(MsCount % Scrollspeed == 0){ //scrollspeed
    TempPosition++;  
  }
};

prog_uchar Tekens[475] PROGMEM = //ascii tekens 32-126
{   
          B00000000,//32 - space
          B00000000,
          B00000000,
          B00000000,
          B00000000,
    
          B00000000,//33
          B00000000,
          B01011111,
          B00000000,
          B00000000,
    
          B00000000,//34
          B00000111,
          B00000000,
          B00000111,
          B00000000,
    
          B00010100,//35
          B01111111,
          B00010100,
          B01111111,
          B00010100,
    
          B00100100,//36
          B00101010,
          B01111111,
          B00101010,
          B00010010,
    
          B00100011,//37
          B00010011,
          B00001000,
          B01100100,
          B01100010,
    
          B00110110,//38
          B01001001,
          B01010101,
          B00100010,
          B01010000,
    
          B00000000,//39
          B00000101,
          B00000011,
          B00000000,
          B00000000,
    
          B00000000,//40
          B00011100,
          B00100010,
          B01000001,
          B00000000,
    
          B00000000,//41
          B01000001,
          B00100010,
          B00011100,
          B00000000,
    
          B00001000,//42
          B00101010,
          B00011100,
          B00101010,
          B00001000,
    
          B00001000,//43
          B00001000,
          B00111110,
          B00001000,
          B00001000,
    
          B00000000,//44
          B01010000,
          B00110000,
          B00000000,
          B00000000,
    
          B00001000,//45
          B00001000,
          B00001000,
          B00001000,
          B00001000,
    
          B00000000,//46
          B01100000,
          B01100000,
          B00000000,
          B00000000,
    
          B00100000,//47
          B00010000,
          B00001000,
          B00000100,
          B00000010,
    
          B00111110,//48
          B01010001,
          B01001001,
          B01000101,
          B00111110,
    
          B00000000,//49
          B01000010,
          B01111111,
          B01000000,
          B00000000,
    
          B01000010,//50
          B01100001,
          B01010001,
          B01001001,
          B01000110,
    
          B00100001,//51
          B01000001,
          B01000101,
          B01001011,
          B00110001,
    
          B00011000,//52
          B00010100,
          B00010010,
          B01111111,
          B00010000,
    
          B00100111,//53
          B01000101,
          B01000101,
          B01000101,
          B00111001,
    
          B00111100,//54
          B01001010,
          B01001001,
          B01001001,
          B00110000,
    
          B00000001,//55
          B01110001,
          B00001001,
          B00000101,
          B00000011,
    
          B00110110,//56
          B01001001,
          B01001001,
          B01001001,
          B00110110,
    
          B00000110,//57
          B01001001,
          B01001001,
          B00101001,
          B00011110,
    
          B00000000,//58
          B00110110,
          B00110110,
          B00000000,
          B00000000,
    
          B00000000,//59
          B01010110,
          B00110110,
          B00000000,
          B00000000,
    
          B00000000,//60
          B00001000,
          B00010100,
          B00100010,
          B01000001,
		  
		   B00010100,//61
          B00010100,
          B00010100,
          B00010100,
          B00010100,
    
          B01000001,//62
          B00100010,
          B00010100,
          B00001000,
          B00000000,
    
          B00000010,//63
          B00000001,
          B01010001,
          B00001001,
          B00000110,
  
          B00110010,//64
          B01001001,
          B01111001,
          B01000001,
          B00111110,
    
          B01111110,//65
          B00010001,
          B00010001,
          B00010001,
          B01111110,
    
          B01111111,//66
          B01001001,
          B01001001,
          B01001001,
          B00110110,
    
          B00111110,//67
          B01000001,
          B01000001,
          B01000001,
          B00100010,
    
          B01111111,//68
          B01000001,
          B01000001,
          B00100010,
          B00011100,
    
          B01111111,//69 
          B01001001,
          B01001001,
          B01001001,
          B01000001,
    
          B01111111,//70 F
          B00001001,
          B00001001,
          B00000001,
          B00000001,
   
          B00111110,//71
          B01000001,
          B01000001,
          B01010001,
          B00110010,
    
          B01111111,//72
          B00001000,
          B00001000,
          B00001000,
          B01111111,
    
          B00000000,//73
          B01000001,
          B01111111,
          B01000001,
          B00000000,
    
          B00100000,//74
          B01000000,
          B01000001,
          B00111111,
          B00000001,
    
          B01111111,//75
          B00001000,
          B00010100,
          B00100010,
          B01000001,
    
          B01111111,//76
          B01000000,
          B01000000,
          B01000000,
          B01000000,
    
          B01111111,//77
          B00000010,
          B00000100,
          B00000010,
          B01111111,
    
          B01111111,//78
          B00000100,
          B00001000,
          B00010000,
          B01111111,
    
          B00111110,//79
          B01000001,
          B01000001,
          B01000001,
          B00111110,
    
          B01111111,//80
          B00001001,
          B00001001,
          B00001001,
          B00000110,
    
          B00111110,//81
          B01000001,
          B01010001,
          B00100001,
          B01011110,
    
          B01111111,//82
          B00001001,
          B00011001,
          B00101001,
          B01000110,
    
          B01000110,//83
          B01001001,
          B01001001,
          B01001001,
          B00110001,
    
          B00000001,//84
          B00000001,
          B01111111,
          B00000001,
          B00000001,
    
          B00111111,//85
          B01000000,
          B01000000,
          B01000000,
          B00111111,
    
          B00011111,//86
          B00100000,
          B01000000,
          B00100000,
          B00011111,
    
          B01111111,//87
          B00100000,
          B00011000,
          B00100000,
          B01111111,
    
          B01100011,//88
          B00010100,
          B00001000,
          B00010100,
          B01100011,
    
          B00000011,//89
          B00000100,
          B01111000,
          B00000100,
          B00000011,
    
          B01100001,//90
          B01010001,
          B01001001,
          B01000101,
          B01000011,
    
          B00000000,//91
          B00000000,
          B01111111,
          B01000001,
          B01000001,
    
          B00000010,//92
          B00000100,
          B00001000,
          B00010000,
          B00100000,
    
          B01000001,//93
          B01000001,
          B01111111,
          B00000000,
          B00000000,
    
          B00000100,//94
          B00000010,
          B00000001,
          B00000010,
          B00000100,
    
          B01000000,//95
          B01000000,
          B01000000,
          B01000000,
          B01000000,

          B00000000,//96
          B00000001,
          B00000010,
          B00000100,
          B00000000,
    
          B00100000,//97
          B01010100,
          B01010100,
          B01010100,
          B01111000,
    
          B01111111,//98
          B01001000,
          B01000100,
          B01000100,
          B00111000,
    
          B00111000,//99
          B01000100,
          B01000100,
          B01000100,
          B00100000,
    
          B00111000,//100
          B01000100,
          B01000100,
          B01001000,
          B01111111,
		  
		   B00111000,//101
          B01010100,
          B01010100,
          B01010100,
          B00011000,
    
          B00001000,//102
          B01111110,
          B00001001,
          B00000001,
          B00000010,
    
          B00001000,//103
          B00010100,
          B01010100,
          B01010100,
          B00111100,
    
          B01111111,//104
          B00001000,
          B00000100,
          B00000100,
          B01111000,
    
          B00000000,//105
          B01000100,
          B01111101,
          B01000000,
          B00000000,
    
          B00100000,//106
          B01000000,
          B01000100,
          B00111101,
          B00000000,
    
          B00000000,//107
          B01111111,
          B00010000,
          B00101000,
          B01000100,
    
          B00000000,//108
          B01000001,
          B01111111,
          B01000000,
          B00000000,
    
          B01111100,//109
          B00000100,
          B00011000,
          B00000100,
          B01111000,
    
          B01111100,//110
          B00001000,
          B00000100,
          B00000100,
          B01111000,
    
          B00111000,//111
          B01000100,
          B01000100,
          B01000100,
          B00111000,

          B01111100,//112
          B00010100,
          B00010100,
          B00010100,
          B00001000,
    
          B00001000,//113
          B00010100,
          B00010100,
          B00011000,
          B01111100,
    
          B01111100,//114
          B00001000,
          B00000100,
          B00000100,
          B00001000,
    
          B01001000,//115
          B01010100,
          B01010100,
          B01010100,
          B00100000,
    
          B00000100,//116
          B00111111,
          B01000100,
          B01000000,
          B00100000,
    
          B00111100,//117
          B01000000,
          B01000000,
          B00100000,
          B01111100,
    
          B00011100,//118
          B00100000,
          B01000000,
          B00100000,
          B00011100,
    
          B00111100,//119
          B01000000,
          B00110000,
          B01000000,
          B00111100,
    
          B01000100,//120
          B00101000,
          B00010000,
          B00101000,
          B01000100,
    
          B00001100,//121
          B01010000,
          B01010000,
          B01010000,
          B00111100,
    
          B01000100,//122
          B01100100,
          B01010100,
          B01001100,
          B01000100,
    
          B00000000,//123
          B00001000,
          B00110110,
          B01000001,
          B00000000,
    
          B00000000,//124
          B00000000,
          B01111111,
          B00000000,
          B00000000,
    
          B00000000,//125
          B01000001,
          B00110110,
          B00001000,
          B00000000,
    
          B00001000,//126
          B00001000,
          B00101010,
          B00011100,
          B00001000};


void Helios_LEDS::Init()
{
	pinMode(LedEnable,OUTPUT);
	pinMode(LedLatch,OUTPUT);
	digitalWrite(LedEnable,LOW);
	digitalWrite(LedLatch,LOW);
	SPI.begin();

	Helios_LEDS::ConfigTimer();
}
	
void Helios_LEDS::ConfigTimer()
{   
    TCCR1B = B00000011; //set prescaler B1:0;Set  
	TCCR1A = B00000000; //MODE0
	TIMSK1 |= (1<<TOIE1) |(0<<OCIE1A); //en T1OVF, dis T1 comp
   
	SREG = B10000000;
}

void Helios_LEDS::LedMatrixClear()
{ 
	UpdateText = false;
	Looop = false;
	Done = false;
	StartPos = true;
	for(int i=0; i<Buflength+1;i++){
		AsciiValues[i] = 0;
	}
	for(int i=0;i<(Buflength+1)*6;i++){
		Temp[i] = 0;
	}
}

void Helios_LEDS::LedMatrixScrollSpeed(int ps)
{
	if(ps > 1000) ps = 1000;
	if(ps < 1) ps = 1;
	Scrollspeed = 1000/ps;
}
	
void Helios_LEDS::LedMatrixWrite(char *text)
{
	digitalWrite(LedEnable,LOW);
	Looop = false;
	Done = false;
	strcpy(AsciiValues,text);
	if(text[strlen(text)-1]!=' ') strcat(AsciiValues," "); //voeg spatie toe aan AsciiValues[]
	Buflength = strlen(AsciiValues);
	if(Buflength > 31){
		Buflength = 31;
		AsciiValues[30] = ' ';
	}
	digitalWrite(LedEnable,LOW);
	UpdateText=true;
}
	
void Helios_LEDS::LedMatrixWrite(char *text, int ps)
{
	Helios_LEDS::LedMatrixScrollSpeed(ps);
	Helios_LEDS::LedMatrixWrite(text);
}

void Helios_LEDS::LedMatrixLoop(char *text)
{
	digitalWrite(LedEnable,LOW);
	Looop = true;
	strcpy(AsciiValues,text);
	Buflength = strlen(AsciiValues);
	if(Buflength > 31){
		Buflength = 31;
		AsciiValues[30] = ' ';
	}
	ResetTempPos = Buflength*6+5;
	digitalWrite(LedEnable,LOW);
	UpdateText=true;
}

void Helios_LEDS::LedMatrixLoop(char *text, int ps)
{
	Helios_LEDS::LedMatrixScrollSpeed(ps);
	Helios_LEDS::LedMatrixLoop(text);
}
	
bool Helios_LEDS::LedMatrixBusy()
{
	return !Done;
}

bool Helios_LEDS::LedMatrixReady()
{	
	return Done;
}

void Helios_LEDS::LedMatrixWaitTillReady()
{
	while(!Done){}
}

void Helios_LEDS::SetRGB(int r, int g, int b)
{
	digitalWrite(LedEnable,LOW);
	if(r>=1){
		r = 32;
	}
	else{
		r = 0;
	}
	if(g>=1){
		g = 16;
	}
	else{
		g = 0;
	}
	if(b>=1){
		b = 8;
	}
	else{
		b = 0;
	}
	Kleur = r + g + b;
}

void Helios_LEDS::SetLEDS(int a, int b, int c)
{
	digitalWrite(LedEnable,LOW);
	if(a>=1){
		a = 1;
	}
	else{
		a = 0;
	}
	if(b>=1){
		b = 2;
	}
	else{
		b = 0;
	}
	if(c>=1){
		c = 4;
	}
	else{
		c = 0;
	}
	Ledvalue = a + b + c;
}


void Helios_LEDS::ClearAll()
{
	digitalWrite(LedEnable,HIGH);
	Kleur = 0;
	Ledvalue = 0;
	Waarde1 = false;
	Led = false;
	Helios_LEDS::LedMatrixClear();
}

unsigned int Helios_LEDS::Reverse(unsigned int value)
{
	unsigned int rbyte = 0;

	int i;

	for(i=0;i<8;i++){
		if(value & (1<<i)){
			rbyte |= (1<<(7-i));
		}
	}
	return rbyte;
}

void Helios_LEDS::GetTekens()
{
	unsigned int value;
	Temp[0] = 0;
	Temp[1] = 0;
	Temp[2] = 0;
	Temp[3] = 0;
	Temp[4] = 0;
  for(int i = 0;i<Buflength;i++){
    for(int j=0;j<6;j++){
      if(j<5){
		value = pgm_read_byte_near(Tekens + (((AsciiValues[i]-32)*5)+j));
		value = Helios_LEDS::Reverse(value); //INVERT BINAIR VALUES
		Temp[(i*6+5)+j] = value;
      }
      if(j==5){ //ruimte tussen characters toevoegen
        Temp[(i*6+5)+j] = 0;
      }
    }
  }
}

void Helios_LEDS::WriteMatrix() //functie om naar ledmatrix te schrijven
{ 	
  if(UpdateText)
	{
		if(StartPos)
		{
			StartPos = false;
			GetTekens();
			CountLine=0;
			TempPosition = 0;
			Done = false;
		}
		if(!Looop)
		{
			if(TempPosition>Buflength*6){Done = true; UpdateText = false;StartPos = true;}//reset alle variablen 
			SPI.transfer(16>>(CountLine));
			SPI.transfer((Temp[((TempPosition + CountLine))]));
			digitalWrite(LedLatch,HIGH);
			digitalWrite(LedLatch,LOW);  
		} //begin weer te schrijven bij lijn 0 
		if(Looop)
		{	
			SPI.transfer(16>>(CountLine));
			SPI.transfer((Temp[((TempPosition+CountLine) % ResetTempPos)]));
			digitalWrite(LedLatch,HIGH);
			digitalWrite(LedLatch,LOW); 
		}
	}
  if(!UpdateText){  //dim de leds
		SPI.transfer(16>>(CountLine));
		SPI.transfer(0x00);
		digitalWrite(LedLatch,HIGH);
		digitalWrite(LedLatch,LOW);
	}
}

void Helios_LEDS::WriteLed()
{
	SPI.transfer(1<<(5));
	SPI.transfer(Kleur + Ledvalue);
	digitalWrite(LedLatch,HIGH);
	digitalWrite(LedLatch,LOW);
}

	
