/* MaxMatrix2
 * Version 0.1 OCT 2013
 * Copyright 2013 Nikolai Neff
 * based upon MaxMatrix by Oscar Kin-Chung Au
 *
 * Tested with Arduino 1.0.5 and 1.5.4 on Arduino Uno Rev3
 *
 * This file is part of MaxMatrix2
 *
 * MaxMatrix2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MaxMatrix2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MaxMatrix2.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MaxMatrix2_H_
#define _MaxMatrix2_H_

#include "Arduino.h"

#define max7219_reg_noop        0x00
#define max7219_reg_digit0      0x01
#define max7219_reg_digit1      0x02
#define max7219_reg_digit2      0x03
#define max7219_reg_digit3      0x04
#define max7219_reg_digit4      0x05
#define max7219_reg_digit5      0x06
#define max7219_reg_digit6      0x07
#define max7219_reg_digit7      0x08
#define max7219_reg_decodeMode  0x09
#define max7219_reg_intensity   0x0a
#define max7219_reg_scanLimit   0x0b
#define max7219_reg_shutdown    0x0c
#define max7219_reg_displayTest 0x0F
#define matrixSize				8

class MaxMatrix2
{
	private:
        byte data;
        byte load;
        byte clock;
        byte numDisplays;
		bool transmissionActive;

		void sendData(byte adress, byte data, bool end=false);

	public:
		MaxMatrix2(byte _data, byte _load, byte _clock, byte _numDisplays);

		void init(byte scanLimit=0x07, byte decodeMode=0x00, byte intensity=0x00, bool displayTest=false);
		void setIntensity(byte display, byte intensity);
		void setDecodeMode(byte display, byte decodeMode);
		void setShutdown(byte display, byte shutdown);
		void setTestMode(byte display, byte testMode);
		void clear(byte display);
		void clearAll();
		void sendCommand(byte display, byte registerAddr, byte value);
		void sendArray(byte display, byte buffer[matrixSize]);
		void sendASCIIText(String text);
};
#endif
