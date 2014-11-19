/*
  MD_KeySwitch.cpp - Library for user keyswitch on digital input.

  See main header file for information.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
 */

#include "MD_KeySwitch.h"

MD_KeySwitch::MD_KeySwitch(uint8_t pin) : 
_pin(pin), _onState(KEY_ON_STATE), _state(0)
{
	setDebounceTime(KEY_DEBOUNCE_TIME);
	setRepeatTime(KEY_REPEAT_TIME);
}

MD_KeySwitch::MD_KeySwitch(uint8_t pin, uint8_t onState) :
_pin(pin), _onState(onState), _state(0)
{
	setDebounceTime(KEY_DEBOUNCE_TIME);
	setRepeatTime(KEY_REPEAT_TIME);
}

void MD_KeySwitch::begin(void)
{
	if (_onState == LOW)
		pinMode(_pin, INPUT_PULLUP);
	else
		pinMode(_pin, INPUT);	
}

bool MD_KeySwitch::read(void)
// return true when a transition from OFF to ON states has been detected
// or auto repeat timer has expired and the key is still on
{
	bool b = (digitalRead(_pin) == _onState);

	switch (_state)
	{
		case 0:		// waiting for first transition
			if (b)
			{
				_state = 1;		// edge detected, initiate debounce
				_timeActive = millis();
			}
			break;
		
		case 1:		// debounce time
			b = false;
			if ((millis() - _timeActive) < _timeDebounce)
				break;
		
			// after debounce - possible auto repeat
			_state = 2;
			_timeActive = millis();
			break;
		
		case 2:		// auto repeat
			if (!b)
			{
				_state = 0;		// key off, go back to waiting
			}
			else if ((millis() - _timeActive) < _timeRepeat)
			{
				b = false;		// no repeat yet
			}
			else
			{
				_timeActive = millis();	// next key repeat starts now
			}
			break;
		
		default:
			_state = 0;
			break;
	}

	return(b);
}
