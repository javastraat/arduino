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

#ifndef CapacitiveSensorDue_h
	#define CapacitiveSensorDue_h
	
	#include "Arduino.h"
	
	// AVR vs SAM implementation...
	#ifdef __SAM3X8E__
		#define IS_SAM
		#define REGISTER_DATA_TYPE RwReg*
		#define PORT_DATA_TYPE Pio*
		#define SET_RECEIVE_PIN_TO_INPUT() pinMode(this->_receivePin, INPUT)
		#define SET_RECEIVE_PIN_TO_OUTPUT() pinMode(this->_receivePin, OUTPUT)
		#define SET_SEND_PIN_TO_OUTPUT() pinMode(this->_sendPin, OUTPUT)
	#else
		#define IS_AVR
		#define REGISTER_DATA_TYPE uint8_t*
		#define PORT_DATA_TYPE uint8_t
		#define SET_RECEIVE_PIN_TO_INPUT() *this->_receiveModeRegister &= ~this->_receiveBitmask
		#define SET_RECEIVE_PIN_TO_OUTPUT() *this->_receiveModeRegister |= this->_receiveBitmask
		#define SET_SEND_PIN_TO_OUTPUT() *this->_sendModeRegister |= this->_sendBitmask
	#endif
	
	class CapacitiveSensorDue
	{
		public:
			/**
			 * This value is the cost of the loop used by this object to
			 * calculate the absolute capacitance (in arbitrary unit).
			 */
			static const uint8_t LOOP_TIMING_FACTOR;
			
			/**
			 * [Constructor]
			 * Creates a new CapacitiveSensorDue object with the given pin
			 * numbers.
			 */
			CapacitiveSensorDue(uint8_t sendPin, uint8_t receivePin);
			
			/**
			 * [Set Timeout]
			 * This method allows you to change the timeout for any single
			 * capacitance reading sample.
			 */
			static void setTimeout(unsigned long timeout);
			
			/**
			 * [Read Capacitance]
			 * This method requires one parameter, samples, and returns a long
			 * integer containing the absolute capacitance, in arbitrary units.
			 * The samples parameter can be used to increase the returned
			 * resolution, at the expense of slower performance. The returned
			 * value is not averaged over the number of samples, and the total
			 * value is reported.
			 * 
			 * It will return -1 if the capacitance value exceeds the value of
			 * _timeout.
			 */
			long read(uint8_t samples);
			
			/**
			 * [Calibrate]
			 * This method resets the _leastReadTime to the highest value so
			 * that it will be overwritten on the next reads.
			 */
			void calibrate();
		private:
			/**
			 * Timeout. (default: 2000)
			 */
			static unsigned long _timeout;
			
			/**
			 * Send pin number.
			 */
			 
			uint8_t _sendPin;
			
			/**
			 * Receive pin number.
			 */
			uint8_t _receivePin;
			
			/**
			 * Send pin bitmask. (for fast pin access)
			 */
			uint8_t _sendBitmask;
			
			#ifdef IS_AVR
			/**
			 * Send pin mode register. (for fast pin access)
			 */
			volatile REGISTER_DATA_TYPE _sendModeRegister;
			#endif
			
			/**
			 * Send pin output register. (for fast pin access)
			 */
			volatile REGISTER_DATA_TYPE _sendOutRegister;
			
			/**
			 * Receive pin bitmask. (for fast pin access)
			 */
			uint8_t _receiveBitmask;
			
			#ifdef IS_AVR
			/**
			 * Receive pin mode register. (for fast pin access)
			 */
			volatile REGISTER_DATA_TYPE _receiveModeRegister;
			#endif
			
			/**
			 * Receive pin input register. (for fast pin access)
			 */
			volatile REGISTER_DATA_TYPE _receiveInRegister;
			
			/**
			 * Receive pin output register. (for fast pin access)
			 */
			volatile REGISTER_DATA_TYPE _receiveOutRegister;
			
			/**
			 * The least read time. This is used to keep the "untouched"
			 * value as close as possibile to zero.
			 */
			unsigned long _leastReadTime;
			
			/**
			 * [Sense Sample]
			 * This method calculates a single sample and adds it to the given
			 * total.
			 */
			int senseSample(long *total);
	};
#endif
