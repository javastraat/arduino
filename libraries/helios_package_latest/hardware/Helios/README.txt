Helios drivers/library package version 0.3c
More information at www.heliosgadget.nl

0.3c
* Some tweaks are made in Helios_LEDS and Helios_Colour_Sensor_TCS3414
* Changed WiFlyHQ library examples so that they are direct compatable with Helios (changed Software Serial to Hardware Serial) 

0.3b
* Changed #include "arduino.h" to #include "Arduino.h" in Helios specific libraries (thanks, Ardillo)
* Added lux_meting sketch

0.3a
* LED library name is changed to BEURS_LED (for beurs specific sketches)
* Added BEURS_Colour_Sensor_TCS3414 library (for beurs specific sketches)
* Added copyright to the libraries
* functional_test sketch changed so it will run with BEURS_LED library

0.3
* Extended EEPROM: Name changed to "Helios_EEPROM_24AA014"
* Added Helios_EEPROM_24AA014 example sketch 
* Added Helios library: To shutdown the Helios
* Added Helios_Colour_Sensor_TCS3414 library & example sketch
* Added Helios_LEDS library (main led library) & example sketch 
* Added Helios_RTC_PCF8563 library (real time clock libray) & example sketches
* Added Helios_Temperature_Sensor_TMP006 & example sketch
* Added WiFly library (smaller than WiFlyHQ library, more control, less features)
* Added WiFly examples
  * Change firmware version (update/downgrade) 
  * Serial communication to Wifly

0.2b
* Functionele test sketch added

0.2a
* Drivers updated to Arduino 1.0.4
* WiflyHQ added

0.2 -
* BTN now works! 
* Included Helios LED library v0.5
* Included Extended EEPROM library 

Board.txt
* Changed board name to Helios

0.1 -
* Helios Beta drivers 
