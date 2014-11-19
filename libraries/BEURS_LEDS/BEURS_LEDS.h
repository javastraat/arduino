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

#ifndef _BEURS_LEDS_h
#define _BEURS_LEDS_h

class BEURS_LEDS{
	public:
		void Init();
		void LedkrantWaarde(unsigned int waarde);
		void LedkrantCheckLed(unsigned int waarde);
		void LedkrantScrollTime(int ms);
		void LedkrantWrite(char *text);
		void LedkrantWrite(char *text, int ms);
		void LedkrantLoop(char *text);
		void LedkrantLoop(char *text, int ms);
		bool LedkrantBusy();
		bool LedkrantReady();
		void LedkrantClear();
		void SetRGB(int r, int g, int b);
		void SetLED1(int aan);
		void SetLED2(int aan);
		void SetLED3(int aan);
		void SetLEDS(int a, int b, int c);
		void ClearAll();
		static void WriteMatrix();
		static void WriteLed();
		static void GetTekens();
		static unsigned int Reverse(unsigned int value);
		void ResetButton();
		bool Button();
	private:
		void ConfigTimer();
		
		

};

#endif