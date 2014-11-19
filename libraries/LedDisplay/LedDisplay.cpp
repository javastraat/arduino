/*
 * LedDisplay.cpp
 *
 *  Created on: 11.04.2010
 *      Author: Kristian
 *
 *  All coordinates are 1-based!
 */

#include "LedDisplay.h"
#include "../LedControl/LedControl.h"

LedDisplay::LedDisplay( int dataPin, int clkPin, int csPin )
	: LedControl( dataPin, clkPin, csPin, 4 ) {
	/*
	 * constructor.
	 */

	for(int i = 0; i < 4; i++)
	{
		//set default modes
		shutdown(i, false);
		setIntensity(i, 8);
	}
}
LedDisplay::~LedDisplay()
{
	/*
	 * destructor.
	 */
}

bool LedDisplay::getState ( byte x, byte y )
{
	/*
	 * Returns the state of the LED at the given position.
	 */
	bool state = bitRead( _display_state[ y - 1 ], 16 - x );
	return state;
}

void LedDisplay::setState ( byte x, byte y, bool state )
{
	/*
	 * Sets a single LED to given target state.
	 */
	bitWrite( _display_state[ y - 1 ], 16 - x, state );
}


void LedDisplay::allOff()
{
	for(int i = 0; i < 16; i++){
		_display_state[i] = 0;
	}
}
void LedDisplay::allOn()
{
	for(int i = 0; i < 16; i++){
		_display_state[i] = 0b1111111111111111;
	}
}



void LedDisplay::pushData ()
{
	/*
	 * updates the whole display with the data stored in _display_state.
	 */
	unsigned short int data;
	byte reg;
	byte col;

	for( int k = 0; k < 8; k++ )
	{
		for( int i = 3; i >= 0; i-- )
		{
			//i is the chip id. we start with the last one.
			if( i < 2 )
			{
				//the chips at the top (A0 and B1) use the first eight array indexes
				data = _display_state[k];
			}
			else
			{
				data = _display_state[k + 8];
			}

			if( i % 2 == 0 )
			{
				//A0 and C2, left side, use the eight more significant bits from data
				col = data >> 8;
			}
			else
			{
				col = data & 255;
			}

			reg = k;

			setRow(i, reg, col);
		}
	}
}

void LedDisplay::importData ( unsigned short int _display_state[16] )
{
	//todo: nothing yet.
}


void LedDisplay::tmpDisplayData ()
{
	for(int i = 1; i <= 16; i++){
		for(int k = 1; k <= 16; k++)
		{
			Serial.print( getState(i, k) );
		}
		Serial.println();
	}
}
