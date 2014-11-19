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

#ifndef _Helios_EEPROM_24AA014_h
#define _Helios_EEPROM_24AA014_h

class Helios_EEPROM_24AA014{
	public:
			void ByteWrite(int bytenumber, int data);
			int ByteRead(int bytenumber);
			void IntegerWrite(int bytenumber, unsigned int data);
			unsigned int IntegerRead(int bytenumber);
			int StringWrite(int stringnumber, char *data);
			char *StringRead(int stringnumber);
	private:
			void _Write(int adress,unsigned int data);
			unsigned int _Read(int adress);
			void _PageWrite(int adress, char *data);
			char *_PageRead(int adress);
};
#endif