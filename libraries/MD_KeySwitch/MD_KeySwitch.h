/*
  MD_KeySwitch.h - Library for user keyswitch on digital input.
  
  Created by Marco Colli March 2014
	
  Small library to allow the use of momentary push switches as user
  input devices. 
  - Detects the transition from ON state to OFF state (either low/high or high/low)
  - Software debounce
  - Software auto repeat
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
 */
#ifndef	MD_KEYSWITCHH
#define	MD_KEYSWITCHH

#include <Arduino.h>

// Default values
#define	KEY_DEBOUNCE_TIME	20	// in milliseconds
#define	KEY_REPEAT_TIME		500	// in milliseconds
#define KEY_ON_STATE		LOW	// default active low - transition high to low

class MD_KeySwitch
{
public:
	MD_KeySwitch(uint8_t pin);
	MD_KeySwitch(uint8_t pin, uint8_t onState);
	
	~MD_KeySwitch() { };
		
	void setDebounceTime(uint16_t t) { _timeDebounce = t; };
	void setRepeatTime(uint16_t t)	{ _timeRepeat = t; };	

	void begin(void);
	bool read(void);
	
protected:
	uint8_t		_pin;			// the pin number
	uint8_t		_onState;		// the digital state for ON
	uint8_t		_state;			// the last state (FSM)
	uint32_t	_timeActive;	// the millis() time it was last activated
	uint16_t	_timeDebounce;	// debounce time in milliseconds
	uint16_t	_timeRepeat;	// repeat time delay in milliseconds
};

#endif