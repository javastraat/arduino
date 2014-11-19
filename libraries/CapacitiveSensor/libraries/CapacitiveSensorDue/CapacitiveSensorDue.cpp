/**
 * Capacitive Sensing Library for 'duino / Wiring
 * Copyright (c) 2008 Paul Bagder  All rights reserved.
 * Version 04 by Paul Stoffregen - Arduino 1.0 compatibility, issue 146 fix
 * Version "Due-compatible" by Marco Lipparini
 * 
 * This file is part of the MLX Hardware project.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this code.
 * If not, see <http://www.gnu.org/licenses/gpl-2.0.html>.
 *
 * Project website: http://www.mlx-service.it
 */

// Required libraries...
#include "CapacitiveSensorDue.h"
#include "Arduino.h"
#include <limits.h>

/*
 * Static fields...
 */
// Value determined empirically by the original author.
const uint8_t CapacitiveSensorDue::LOOP_TIMING_FACTOR = 310;

// Using floats to deal with large numbers.
#define TIMEOUT_FORMULA(TIMEOUT) (TIMEOUT * (float) CapacitiveSensorDue::LOOP_TIMING_FACTOR)

// Default timeout.
unsigned long CapacitiveSensorDue::_timeout = TIMEOUT_FORMULA(2000);

/*
 * Methods...
 */
// [Constructor]
CapacitiveSensorDue::CapacitiveSensorDue(uint8_t sendPin, uint8_t receivePin)
{
	PORT_DATA_TYPE sendPort;
	PORT_DATA_TYPE receivePort;
	
	this->_sendPin = sendPin;
	this->_receivePin = receivePin;
	this->calibrate();
	
	// Initializing send pin...
	this->_sendBitmask = digitalPinToBitMask(sendPin);
	sendPort = digitalPinToPort(sendPin);
	#ifdef IS_AVR
	this->_sendModeRegister = portModeRegister(sendPort);
	#endif
	this->_sendOutRegister = portOutputRegister(sendPort);
	
	// Initializing receive pin...
	this->_receiveBitmask = digitalPinToBitMask(receivePin);
	receivePort = digitalPinToPort(receivePin);
	#ifdef IS_AVR
	this->_receiveModeRegister = portModeRegister(receivePort);
	#endif
	this->_receiveInRegister = portInputRegister(receivePort);
	this->_receiveOutRegister = portOutputRegister(receivePort);
	
	noInterrupts();
	SET_SEND_PIN_TO_OUTPUT();
	interrupts();
}

// [Set Timeout]
void CapacitiveSensorDue::setTimeout(unsigned long timeout) {
	CapacitiveSensorDue::_timeout = TIMEOUT_FORMULA(timeout);
}

// [Read Capacitance]
long CapacitiveSensorDue::read(uint8_t samples)
{
	long total = 0;
	
	if (samples == 0) {
		return 0;
	}
	
	for (uint8_t i = 0; i < samples; i++) {
		if (senseSample(&total) < 0) {
			// Timeout...
			return -1;
		}
	}
	
	if (total < this->_leastReadTime) {
		this->_leastReadTime = total;
	}
	
	return total - this->_leastReadTime;
}

// [Calibrate]
void CapacitiveSensorDue::calibrate() {
	this->_leastReadTime = ULONG_MAX;
}

// [Sense Sample]
int CapacitiveSensorDue::senseSample(long *total)
{
	noInterrupts();
	
	// Set send pin register LOW...
	*this->_sendOutRegister &= ~this->_sendBitmask;
	SET_RECEIVE_PIN_TO_INPUT();
	// Set receive pin register LOW to make sure pullups are off...
	*this->_receiveOutRegister &= ~this->_receiveBitmask;
	SET_RECEIVE_PIN_TO_OUTPUT(); // Receive pin is now LOW AND OUTPUT...
	SET_RECEIVE_PIN_TO_INPUT();
	// Set send pin HIGH
	*this->_sendOutRegister |= this->_sendBitmask;
	
	interrupts();
	
	#define LOOP_CONDITION (*this->_receiveInRegister & this->_receiveBitmask) && (*total < CapacitiveSensorDue::_timeout)
	
	// While receive pin is LOW and timeout has not occurred...
	while (!LOOP_CONDITION) {
		(*total)++;
	}
	
	#define CHECK_TIMEOUT() \
		if (*total >= CapacitiveSensorDue::_timeout) { \
			return -1; \
		}
	CHECK_TIMEOUT();
	
	// Set receive pin HIGH briefly to charge up fully because the while loop above will
	// exit when pin is ~2.5V. 
	noInterrupts();
	
	// Set receive pin HIGH (turns on pullup)...
	*this->_receiveOutRegister |= this->_receiveBitmask;
	SET_RECEIVE_PIN_TO_OUTPUT();
	SET_RECEIVE_PIN_TO_INPUT();
	// Turn off pullup...
	*this->_receiveOutRegister &= ~this->_receiveBitmask;
	// Set send pin LOW
	*this->_sendOutRegister &= ~this->_sendBitmask;
	
	interrupts();
	
	// While receive pin is HIGH and timeout has not occurred...
	while (LOOP_CONDITION) {
		(*total)++;
	}
	
	CHECK_TIMEOUT();
	
	return 1;
}
