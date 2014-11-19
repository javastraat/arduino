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

#include "Arduino.h"
#include "MaxMatrix2.h"

MaxMatrix2::MaxMatrix2(byte _data, byte _load, byte _clock, byte _numDisplays)
{
	data = _data;
	load = _load;
	clock = _clock;
	numDisplays = _numDisplays;
	transmissionActive = false;
}

void MaxMatrix2::init(byte scanLimit, byte decodeMode, byte intensity, bool displayTest)
{
	pinMode(data,  OUTPUT);
	pinMode(clock, OUTPUT);
	pinMode(load,  OUTPUT);

	/* From the MAX7219/MAX7221 Datasheet:

	On initial power-up, all control registers are reset, the
	display is blanked, and the MAX7219/MAX7221 enter
	shutdown mode. Program the display driver prior to
	display use. Otherwise, it will initially be set to scan one
	digit, it will not decode data in the data registers, and
	the intensity register will be set to its minimum value.
	*/

	/*It might take a while for these defaults to kick in,
	especially if the devices power supply is backed up by a capacitor as it is recommended in the manual.
	Therefore, the init function always sends values to all status-registers to make sure no settings from the last power-on cycle remain*/

	byte i;

	for (i=0; i<numDisplays; i++)
	{
		if (i!=numDisplays-1)
		{
			sendData(max7219_reg_scanLimit, scanLimit);
		} else {
			sendData(max7219_reg_scanLimit, scanLimit, true);
		}
	}

	for (i=0; i<numDisplays; i++)
	{
		if (i!=numDisplays-1)
		{
			sendData(max7219_reg_decodeMode, decodeMode);
		} else {
			sendData(max7219_reg_decodeMode, decodeMode, true);
		}
	}

	for (i=0; i<numDisplays; i++)
	{
		if (i!=numDisplays-1)
		{
			sendData(max7219_reg_displayTest, displayTest);
		} else {
			sendData(max7219_reg_displayTest, displayTest, true);
		}
	}

	for (i=0; i<numDisplays; i++)
	{
		if (i!=numDisplays-1)
		{
			sendData(max7219_reg_shutdown, 0x01);
		} else {
			sendData(max7219_reg_shutdown, 0x01, true);
		}
	}
	
	for (i=0; i<numDisplays; i++)
	{
		if (i!=numDisplays-1)
		{
			sendData(max7219_reg_intensity, intensity);
		} else {
			sendData(max7219_reg_intensity, intensity, true);
		}
	}
}

void MaxMatrix2::sendData(byte registerAddr, byte value, bool end) // sends data serially into the shift registers without terminating the transmission afterwards
{
	if (transmissionActive == false)
	{
		digitalWrite(load, LOW); //indicates data transfer start
		transmissionActive = true;
	}

	shiftOut(data, clock, MSBFIRST, registerAddr); //send desired 16 bit of data split up in to parts, as Arduino only supports 8 bit in software-SPI mode
	shiftOut(data, clock, MSBFIRST, value);

	if (end == true)
	{
		digitalWrite(load, HIGH); //indicates data transfer end
		transmissionActive = false;
	}
}


void MaxMatrix2::setIntensity(byte display, byte intensity) //sets the intensity for a specific display
{
	sendCommand(display, max7219_reg_intensity, intensity);
}

void MaxMatrix2::setDecodeMode(byte display, byte decodeMode) //sets the decoding for CODE-B Font for specific digits for a specific display
{
	sendCommand(display, max7219_reg_decodeMode, decodeMode);
}

void MaxMatrix2::setShutdown(byte display, byte shutdown) //shutdown or wake up specific display
{
	sendCommand(display, max7219_reg_shutdown, shutdown);
}

void MaxMatrix2::setTestMode(byte display, byte testMode) //test specific display
{
	sendCommand(display, max7219_reg_displayTest, testMode);
}

void MaxMatrix2::clear(byte display)
{
	for (byte i=1; i<=8; i++) //output registers are 1 to 8
	{
		sendCommand(display, i, 0); //set all outputs to 0
	}
}

void MaxMatrix2::clearAll()
{
	for(byte i=0; i<matrixSize; i++)
	{
		for(byte n=0; n<numDisplays; n++)
		{
			if(n!=numDisplays-1)
			{
				sendData(i+1,0);
			} else {
				sendData(i+1,0,true);
			}
		}
	}
}



void MaxMatrix2::sendCommand(byte display, byte registerAddr, byte value) // sends a command to a specific Display
{
	//for details on the Addressing of specific registers look in the Max 7219 datasheet

	digitalWrite(load, LOW); //indicates data transfer start

	/*cascaded displays act like a shift register. To access a display in the middle of this chain,
	all other displays have to be ignored by sending specific No-OP instructions. */

	for (byte i=0; i<(numDisplays-(display+1)); i++)  //send NoOps for leading Displays
	{
		shiftOut(data, clock, MSBFIRST, max7219_reg_noop);
		shiftOut(data, clock, MSBFIRST, 0x00);
	}

	shiftOut(data, clock, MSBFIRST, registerAddr); //send desired 16 bit of data split up in to parts, as Arduino only supports 8 bit in software-SPI mode
	shiftOut(data, clock, MSBFIRST, value);

	for (byte i=0; i<display; i++) //send NoOps for trailing Displays
	{
		shiftOut(data, clock, MSBFIRST, max7219_reg_noop);
		shiftOut(data, clock, MSBFIRST, 0x00);
	}

	digitalWrite(load, HIGH); //indicates data transfer end
}

void MaxMatrix2::sendArray(byte display, byte buffer[matrixSize])
{
	for (byte i=0; i<matrixSize; i++)
	{
		sendCommand(display, i+1, buffer[i]);
	}
}

void MaxMatrix2::sendASCIIText(String text)
{
    //Todo: implement
//    int col = 0;
//    while (text != 0)
//    {
//        if (text < 32) //ignore control caracters
//        {
//            continue;
//        }
//        char c = *s - 32;
//
//        memcpy_P(buffer, CH + 7*c, 7);
//        m.writeSprite(col, 0, buffer);
//        m.setColumn(col + buffer[0], 0);
//        col += buffer[0] + 1;
//        s++;
//    }
}
