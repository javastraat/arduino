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

#ifndef _Helios_RTC_PCF8563_h
#define _Helios_RTC_PCF8563_h

class Helios_RTC_PCF8563{
	public:			
			void SetAll(int hours, int minutes, int seconds, int weekdays, int day, int month, int year); //
			void SetSmall(int hours,int minutes,int seconds,int day,int month, int year); //
			void SetTime(int hours, int minutes, int seconds);//
			void SetDate(int day, int month, int year);//
			void SetWeekday(int weekdays);//
			char *ReadTime();//return time in string
			char *ReadDate();//return date in string
			char *ReadAll();// return date and time in string
			char *ReadSmall();
			int GetSeconds();// return seconds as integer
			int GetMinutes();// return minutes as integer
			int GetHours();// return hours as integer
			 
			void SetAlarmDaily(int hours, int minutes);
			void SetAlarmWeekly(int hours,int minutes, int weekdays);
			void SetAlarmMonthly(int hours,int minutes, int days);		 
			void DisableAlarm(); // disable interrupts, clear flags, disable all alarms
			void ResetAlarm(); // alarm interrupt enable, clear flags
			bool ReadAlarmFlag(); //read controll_status_2
			
			void SetTimer(int minutes); // countdown value in minutes
			void DisableTimer(); //disabel intterupts, clear flags
			void ResetTimer(); //timer interrupt enable, clear flags
			bool ReadTimerFlag(); //read controll_status_2
			
	private:
			void EnableAlarm(bool onm, bool onh, bool ond, bool onw); //enable alarm interrupt,clear flags,enable alarms(parameters are minutes,hours,days,weekdays(boolean))
			void EnableTimer(int minutes); //enable timer interrupt, clear flags
			char *Dag(int w);	
			int DecToBcd(int val);
};
#endif