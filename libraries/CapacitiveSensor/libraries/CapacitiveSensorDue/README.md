CapacitiveSensorDue
===================
by Marco Lipparini

## Overview
This is an alternative to the **CapacitiveSensor** library which includes:
* Arduino Due compatibility *(Both SAM and AVR chips are compatible)*
* Code completely rewritten and cleaned
* Explicitly released under GPLv2

## How it works
The new library uses the same technique as the old one to read capacitance on a metal object, you can find a great explanation on the original wiki page: http://playground.arduino.cc/Main/CapacitiveSensor.

## Methods
### *static* void setTimeout(unsigned long timeout)
This method sets a new timeout for a single sample, if this value is exceded during a sample then an error is returned. It is in arbitrary units and you can experience with different values to find out what works best with your sketch.

### long read(uint8_t samples)
This method requires one parameter, samples, and returns a long integer containing the absolute capacitance, in arbitrary units. The samples parameter can be used to increase the returned resolution, at the expense of slower performance.

The returned value is not averaged over the number of samples, and the total value is reported minus the value of the minimum capacitance value read until that time.  
It will return **-1** if the capacitance value exceeds the timeout value.

### void calibrate()
This method resets the variable used to take your read value down to ~0 when your object is not being touched.  
That value will be set again on the next reads automatically.

***
**Original Author:** Paul Bagder *(v04 by Paul Stoffregen)*  
**Original Website:** http://playground.arduino.cc/Main/CapacitiveSensor  
**Original Source:** https://github.com/arduino-libraries/CapacitiveSensor