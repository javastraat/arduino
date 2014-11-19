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

#ifndef _Helios_Colour_Sensor_TCS3414_h
#define _Helios_Colour_Sensor_TCS3414_h



class Helios_Colour_Sensor_TCS3414{
	public:
			void Read(unsigned int*l, unsigned int* cct);
			void ReadLow(int t, int ga, int p,unsigned int* r,unsigned int* g,unsigned int* b,unsigned int* c,unsigned int* l, unsigned int* cct);
	private:
			void Init();
			void InitLow(int t, int ga, int p);
			void AutoSet();
			unsigned int TcsGetGreen();
			unsigned int TcsGetRed();
			unsigned int TcsGetBlue();
			unsigned int TcsGetClear();
};

#endif