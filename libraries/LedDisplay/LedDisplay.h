/*
 * disp16x16.h
 *
 *  Created on: 06.03.2010
 *      Author: Kristian
 */

#ifndef LEDDISPLAY_H_
#define LEDDISPLAY_H_

#if defined(ARDUINO) && ARDUINO >= 100
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif
#include <../LedControl/LedControl.h>;

class LedDisplay : public LedControl {
public:
	LedDisplay(int dataPin, int clkPin, int csPin);
	~LedDisplay();

	bool getState ( byte x, byte y );
	void setState ( byte x, byte y, bool state );

	void allOff();
	void allOn();

	void pushData ();
	void importData ( unsigned short int _display_state[16] );

	void tmpDisplayData ();

	unsigned short int _display_state[16];
};

#endif /* LEDDISPLAY_H_ */
