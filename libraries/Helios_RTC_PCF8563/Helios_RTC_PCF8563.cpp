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
 
#include <Helios_RTC_PCF8563.h>
#include <Wire.h>
#include "Arduino.h"

#define RTCAdress B1010001

unsigned int Century  = 2000;
unsigned int mAlarm = 0;
unsigned int hAlarm = 0;
unsigned int dAlarm = 0;
unsigned int wAlarm = 0;
unsigned int Minutes = 1;

int Helios_RTC_PCF8563::DecToBcd(int val)
{
  return ( (val/10*16) + (val%10) );
}

void Helios_RTC_PCF8563::SetSmall(int hours,int minutes,int seconds,int day,int month, int year)
{
    int weekdays  = 0;
	seconds = DecToBcd(seconds);
	minutes = DecToBcd(minutes);
	hours = DecToBcd(hours);
	day = DecToBcd(day);
	month = DecToBcd(month);
	year = DecToBcd(year);
    Wire.beginTransmission(RTCAdress);
    Wire.write(0x02); //begin adress: seconds
    Wire.write(seconds); //set seconds
    Wire.write(minutes); //set minutes
    Wire.write(hours); //set hours
    Wire.write(day); //set days
    Wire.write(weekdays); //set weekday
    Wire.write(month); //set months
    Wire.write(year);// set Years
    Wire.endTransmission();
}
void Helios_RTC_PCF8563::SetAll(int hours, int minutes, int seconds, int weekdays, int day, int month, int year)
{
	if(seconds > 59){seconds = 0;}
	if(seconds < 0){seconds = 0;}
	seconds = DecToBcd(seconds);
	if(minutes > 59){minutes = 0;}
	if(minutes < 0){minutes = 0;}
	minutes = DecToBcd(minutes);
	if(hours > 23){hours = 0;}
	if(hours < 0){hours = 0;}
	hours = DecToBcd(hours);
	if(weekdays > 6){weekdays = 0;}
	if(weekdays < 0){weekdays = 0;}
	weekdays = DecToBcd(weekdays);
	if(day > 31){day = 0;}
	if(day < 1){day = 1;}
	day = DecToBcd(day);
	if(month > 12){month = 0;}
	if(month < 1){month = 1;}
	month = DecToBcd(month);
	if(year > 99){year = 0;}
	if(year < 0){year = 0;}
	year = DecToBcd(year);
	
	Wire.beginTransmission(RTCAdress);
    Wire.write(0x02); //begin adress: seconds
    Wire.write(seconds); //set seconds
    Wire.write(minutes); //set minutes
    Wire.write(hours); //set hours
    Wire.write(day); //set days
    Wire.write(weekdays); //set weekday
    Wire.write(month); //set months
    Wire.write(year);// set Years
    Wire.endTransmission();
}

void Helios_RTC_PCF8563::SetTime(int hours, int minutes, int seconds)
{
	if(seconds > 59){seconds = 0;}
	if(seconds < 0){seconds = 0;}
	seconds = DecToBcd(seconds);
	if(minutes > 59){minutes = 0;}
	if(minutes < 0){minutes = 0;}
	minutes = DecToBcd(minutes);
	if(hours > 23){hours = 0;}
	if(hours < 0){hours = 0;}
	hours = DecToBcd(hours);
	
    Wire.beginTransmission(RTCAdress);
    Wire.write(0x02); //begin adress: seconds
    Wire.write(seconds); //set seconds
    Wire.write(minutes); //set minutes
    Wire.write(hours); //set hours
    Wire.endTransmission();
}

void Helios_RTC_PCF8563::SetDate(int day, int month, int year)
{
	if(day > 31){day = 0;}
	if(day < 1){day = 1;}
	day = DecToBcd(day);
	if(month > 12){month = 0;}
	if(month < 1){month = 1;}
	month = DecToBcd(month);
	if(year > 99){year = 0;}
	if(year < 0){year = 0;}
	year = DecToBcd(year);
	
    Wire.beginTransmission(RTCAdress);
    Wire.write(0x05); //begin adress : day
    Wire.write(day); //set days
    Wire.endTransmission();
    Wire.beginTransmission(RTCAdress);
    Wire.write(0x07); //begin adress : set month
    Wire.write(month); //set months
    Wire.write(year);// set Years
    Wire.endTransmission();
}

void Helios_RTC_PCF8563::SetWeekday(int weekdays)
{
	if(weekdays > 6){weekdays = 0;}
	if(weekdays < 0){weekdays = 0;}
	weekdays = DecToBcd(weekdays);
	
    Wire.beginTransmission(RTCAdress);
    Wire.write(0x06); //begin adress: weekday
    Wire.write(weekdays); //set Day of the week
    Wire.endTransmission();
}


int decode(int offset, int mask,char *result)
{ int v,h,l;
    v = Wire.read();  // 2: seconds
	h = (v>>4) & mask;
	l = v & 0xF; 
	result[offset]='0'+h;
	result[offset+1]='0'+l;

}

char *Helios_RTC_PCF8563::ReadSmall()
{                       //   01234567890123456
	static char result[18]={"  :  :     /  /  "};
	
	Wire.beginTransmission(RTCAdress);
	Wire.write(0x02);
	Wire.endTransmission();
	Wire.beginTransmission(RTCAdress);
	Wire.requestFrom(RTCAdress,7); 

	decode(6,0x7,result);
	decode(3,0x7,result);	
	decode(0,0x3,result);	
	decode(9,0x3,result);	
	Wire.read(); //6: Weekday
	decode(12,0x1,result);	
	decode(15,0xF,result);	
	Wire.endTransmission();
  
	return result;
}

char *Helios_RTC_PCF8563::ReadAll()
{
	static char result[30];
	unsigned int sh, sl, mh, ml, hh, hl, dh, dl, monh, monl, yh, yl;
	unsigned int century = 0;
	
	Wire.beginTransmission(RTCAdress);
	Wire.write(0x02);
	Wire.endTransmission();
  
	Wire.beginTransmission(RTCAdress);
	Wire.requestFrom(RTCAdress,7); 
	unsigned int s = Wire.read();
	unsigned int m = Wire.read();
	unsigned int h = Wire.read();
	unsigned int d = Wire.read();
	unsigned int w = Wire.read();
	unsigned int mon = Wire.read();
	unsigned int y = Wire.read();
	Wire.endTransmission();

	sh = (s>>4) & 0x7;
	sl = s & 0xF; 
	s = sh*10 + sl;
	 
	mh = (m>>4) & 0x7;
	ml = m & 0xF;
	m = mh*10 + ml;
  
	hh = (h>>4) & 0x3;
	hl = h & 0xF;
	h = hh*10 + hl;
	
	dh = (d>>4) & 0x3;
	dl = d & 0xF;
	d = dh*10 + dl;
  
	w = w & 0x7;

	century = (mon>>7) & 0x1;
	monh = (mon>>4) & 0x1;
	monl = mon & 0xF;
	mon = monh*10 + monl;
  
	//you need this in 2100
	/*if(century == 1){  
		unsigned int test = mon & 0x1F;
		Wire.beginTransmission(RTCAdress);
		Wire.write(0x07); //write to Century_Month reg
		Wire.write(test); //Set Century+1=0, Month = Jan(1)
		Wire.endTransmission();
		Century = Century + 100;
	}*/
  
	yh = (y>>4) & 0xF;
	yl = y & 0xF;
	y = yh*10 + yl;
	y = y + Century;
  
	sprintf(result,"%02d:%02d:%02d %s %02d/%02d/%d",h,m,s,Dag(w),d,mon,y);
	
	return result;
}

char *Helios_RTC_PCF8563::ReadTime()
{
	static char result[30];
	unsigned int xh, xl, mh, ml, hh, hl;
	
	Wire.beginTransmission(RTCAdress);
	Wire.write(0x02);
	Wire.endTransmission();
  
	Wire.beginTransmission(RTCAdress);
	Wire.requestFrom(RTCAdress,3); 
	unsigned int xr = Wire.read();
	unsigned int m = Wire.read();
	unsigned int h = Wire.read();
	Wire.endTransmission();
	unsigned int x = xr;

	xh = (xr>>4) & 0x7;
	xl = xr & 0xF; 
	x = xh*10 + xl;
  
	mh = (m>>4) & 0x7;
	ml = m & 0xF;
	m = mh*10 + ml;
  
	hh = (h>>4) & 0x3;
	hl = h & 0xF;
	h = hh*10 + hl;

	sprintf(result,"%02d:%02d:%02d",h,m,x);
	
	return result;
}

char *Helios_RTC_PCF8563::ReadDate()
{
	static char result[30];
	unsigned int dh, dl, monh, monl, yh, yl,century;
	
	Wire.beginTransmission(RTCAdress);
	Wire.write(0x05);
	Wire.endTransmission(); 
	Wire.beginTransmission(RTCAdress);
	Wire.requestFrom(RTCAdress,1); 
	unsigned int d = Wire.read();
	Wire.endTransmission();
	
	Wire.beginTransmission(RTCAdress);
	Wire.write(0x06);
	Wire.endTransmission(); 
	Wire.beginTransmission(RTCAdress);
	Wire.requestFrom(RTCAdress,2); 
	unsigned int mon = Wire.read();
	unsigned int y = Wire.read();
	Wire.endTransmission();

	dh = (d>>4) & 0x3;
	dl = d & 0xF;
	d = dh*10 + dl;
	
	century = (mon>>7) & 0x1;
	monh = (mon>>4) & 0x1;
	monl = mon & 0xF;
	mon = monh*10 + monl;
  
	if(century == 1){
		unsigned int test = mon & 0x1F;
		Wire.beginTransmission(RTCAdress);
		Wire.write(0x07); //write to Century_Month reg
		Wire.write(test); //Set Century+1=0, Month = Jan(1)
		Wire.endTransmission();
		Century = Century + 100;
	}
  
	yh = (y>>4) & 0xF;
	yl = y & 0xF;
	y = yh*10 + yl;
	y = y + Century;
  
	sprintf(result,"%02d/%02d/%d",d,mon,y);
	
	return result;
}

int Helios_RTC_PCF8563::GetSeconds()
{	
	unsigned xh,xl;
	Wire.beginTransmission(RTCAdress);
	Wire.write(0x02);
	Wire.endTransmission(); 
	Wire.beginTransmission(RTCAdress);
	Wire.requestFrom(RTCAdress,1); 
	unsigned int s = Wire.read();
	Wire.endTransmission();
	
	xh = (s>>4) & 0x7;
	xl = s & 0xF; 
	s = xh*10 + xl;
	
	return s;
}

int Helios_RTC_PCF8563::GetMinutes()
{
	unsigned int mh,ml;
	Wire.beginTransmission(RTCAdress);
	Wire.write(0x03);
	Wire.endTransmission(); 
	Wire.beginTransmission(RTCAdress);
	Wire.requestFrom(RTCAdress,1); 
	unsigned int m = Wire.read();
	Wire.endTransmission();
	
	mh = (m>>4) & 0x7;
	ml = m & 0xF;
	m = mh*10 + ml;
	
	return m;

}

int Helios_RTC_PCF8563::GetHours()
{
	unsigned int hh,hl;
	Wire.beginTransmission(RTCAdress);
	Wire.write(0x04);
	Wire.endTransmission(); 
	Wire.beginTransmission(RTCAdress);
	Wire.requestFrom(RTCAdress,1); 
	unsigned int h = Wire.read();
	Wire.endTransmission();
	
	hh = (h>>4) & 0x3;
	hl = h & 0xF;
	h = hh*10 + hl;
	
	return h;
}

void Helios_RTC_PCF8563::SetAlarmDaily(int hours,int minutes)
{
	if(minutes > 59){minutes = 0;}
	if(minutes < 0){minutes = 0;}
	mAlarm = DecToBcd(minutes);
	if(hours > 23){hours = 0;}
	if(hours < 0){hours = 0;}
	hAlarm = DecToBcd(hours);
	EnableAlarm(true,true,false,false);
}

void Helios_RTC_PCF8563::SetAlarmWeekly(int hours,int minutes, int weekdays)
{
	if(minutes > 59){minutes = 0;}
	if(minutes < 0){minutes = 0;}
	mAlarm = DecToBcd(minutes);
	if(hours > 23){hours = 0;}
	if(hours < 0){hours = 0;}
	hAlarm = DecToBcd(hours);
	if(weekdays > 6){weekdays = 0;}
	if(weekdays < 0){weekdays = 0;}
	wAlarm = DecToBcd(weekdays);
	EnableAlarm(true,true,false,true);
}

void Helios_RTC_PCF8563::SetAlarmMonthly(int hours,int minutes,int days)
{
	if(minutes > 59){minutes = 0;}
	if(minutes < 0){minutes = 0;}
	mAlarm = DecToBcd(minutes);
	if(hours > 23){hours = 0;}
	if(hours < 0){hours = 0;}
	hAlarm = DecToBcd(hours);
	if(days > 31){days = 0;}
	if(days < 0){days = 0;}
	dAlarm = DecToBcd(days);
	EnableAlarm(true,true,true,false);
}


void Helios_RTC_PCF8563::EnableAlarm(bool onm, bool onh, bool ond, bool onw)
{
    unsigned int ml,mh,hl,hh,dl,dh,wl,wh,setm,seth,setd,setw;
    Wire.beginTransmission(RTCAdress);
    Wire.write(0x01);
    Wire.write(0x12); //enable interrupt //clear flag
    Wire.endTransmission();
    delay(5);
	ml = mAlarm;
	hl = hAlarm;
	dl = dAlarm;
	wl = wAlarm;
	if(onm){mh = 0;}
	else{mh = 1;}
	if(onh){hh = 0;}
	else{hh = 1;}
	if(ond){dh = 0;}
	else{dh = 1;}
	if(onw){wh = 0;}
	else{wh = 1;}
	setm = ml + (mh << 7);
	seth = hl + (hh << 7);
	setd = dl + (dh << 7);
	setw = wl + (wh << 7);
		
	Wire.beginTransmission(RTCAdress);
    Wire.write(0x09); //begin adress: seconds
    Wire.write(setm); //set seconds
    Wire.write(seth); //set minutes
    Wire.write(setd); //set hours
    Wire.write(setw); //set days
    Wire.endTransmission();
}

void Helios_RTC_PCF8563::ResetAlarm()
{
	Wire.beginTransmission(RTCAdress);
    Wire.write(0x01);
    Wire.write(0x12); //interrupt on, clear flag
    Wire.endTransmission();
    delay(5);
}

void Helios_RTC_PCF8563::DisableAlarm()
{
	Wire.beginTransmission(RTCAdress);
    Wire.write(0x01);
    Wire.write(0x10);  //interrupt off, clear flag
    Wire.endTransmission();
    delay(5);
		
	Wire.beginTransmission(RTCAdress);
    Wire.write(0x09); //begin adress: seconds
    Wire.write(0x80); //set minutesalarm off
    Wire.write(0x80); //set hoursalarm off
    Wire.write(0x80); //set daysalarm off
    Wire.write(0x80); //set weekdaysalarm off
    Wire.endTransmission();
}
	
void Helios_RTC_PCF8563::SetTimer(int minutes)
{
	if(minutes > 255){minutes = 255;}
	if(minutes < 1){minutes = 1;}
	EnableTimer(minutes);
}

void Helios_RTC_PCF8563::EnableTimer(int minutes)
{
	Helios_RTC_PCF8563::DisableTimer();
	Wire.beginTransmission(RTCAdress);
    Wire.write(0x0E); //Timercontrolreg
    Wire.write(0x83); //on,1/60hz countdown timer freq
	Wire.write(minutes); //Countdown value
    Wire.endTransmission();
	delay(5);
	Wire.beginTransmission(RTCAdress);
    Wire.write(0x01); //control status 2
    Wire.write(0x11); //enable timer interrupt,clear flags
    Wire.endTransmission();
    delay(5);
}

void Helios_RTC_PCF8563::DisableTimer()
{
	Wire.beginTransmission(RTCAdress);
    Wire.write(0x0E);
    Wire.write(0x03); //off,1/60hz countdown timer freq
	Wire.write(0);
    Wire.endTransmission();
	delay(5);
	Wire.beginTransmission(RTCAdress);
    Wire.write(0x01); //control status2
    Wire.write(0x10); //disable timer interrupt en clear flags
    Wire.endTransmission();
    delay(5);	
}

void Helios_RTC_PCF8563::ResetTimer()
{
	Wire.beginTransmission(RTCAdress);
    Wire.write(0x01);
    Wire.write(0x11); //interrupt on, clear flag
    Wire.endTransmission();
    delay(5);
}

bool Helios_RTC_PCF8563::ReadAlarmFlag()
{
	boolean alarmflag;
	Wire.beginTransmission(RTCAdress);
	Wire.write(0x01); //control status 2
	Wire.endTransmission(); 
	Wire.beginTransmission(RTCAdress);
	Wire.requestFrom(RTCAdress,1); 
	unsigned int af = Wire.read(); 
	Wire.endTransmission();
	
	af = (af >> 3) & 0x1;
	if(af == 1){alarmflag = true;}
	else{alarmflag = false;}
	return alarmflag;
}

bool Helios_RTC_PCF8563::ReadTimerFlag()
{
	boolean timerflag;
	Wire.beginTransmission(RTCAdress);
	Wire.write(0x01); //control status 2
	Wire.endTransmission(); 
	Wire.beginTransmission(RTCAdress);
	Wire.requestFrom(RTCAdress,1); 
	unsigned int tf = Wire.read(); 
	Wire.endTransmission();
	
	tf = (tf >> 2) & 0x1;
	if(tf == 1){timerflag = true;}
	else{timerflag = false;}
	return timerflag;
}

char *Helios_RTC_PCF8563::Dag(int w)
{
  switch(w){
    case 0:
      return "Sunday";
    case 1:
      return "Monday";
    case 2:
      return "Tuesday";
    case 3:
      return "Wednesday";
    case 4:
      return "Thursday";
    case 5:
      return "Friday";
    case 6:
      return "Saturday";
  }
}

	
