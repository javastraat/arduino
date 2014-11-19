/*
  MAX7219.h - A library for controling Leds with a MAX7219
*/

#ifndef MAX7219_h
#define MAX7219_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class MAX7219 {
 private :
    /* The array for shifting the data to the devices */
    byte spidata[16];
    /* Send out a single command to the device */
    void spiTransfer(int addr, byte opcode, byte data);

    /* We keep track of the led-status for both devices in this array */
    byte status[16];
    /* Data is shifted out of this pin*/
    int SPI_MOSI;
    /* The clock is signaled on this pin */
    int SPI_CLK;
    /* This one is driven LOW for chip selectzion */
    int SPI_CS;
    /* The maximum number of devices we use */
    int maxDevices;
    
 public:
    /* 
     * Create a new controler 
     * Params :
     * dataPin		pin on the Arduino where data gets shifted out
     * clockPin		pin for the clock
     * csPin		pin for selecting the device 
     * numDevices	maximum number of devices that can be controlled
     */
    MAX7219(int dataPin, int clkPin, int csPin, int numDevices=1);

    /*
     * Gets the number of devices attached to this LedControl.
     * Returns :
     * int	the number of devices on this LedControl
     */
    int getDeviceCount();

    /* 
     * Set the shutdown (power saving) mode for the device
     * Params :
     * addr	The address of the display to control
     * status	If true the device goes into power-down mode. Set to false
     *		for normal operation.
     */
    void shutdown(int addr, bool status);

    /* 
     * Set the number of digits (or rows) to be displayed.
     * See datasheet for sideeffects of the scanlimit on the brightness
     * of the display.
     * Params :
     * addr	address of the display to control
     * limit	number of digits to be displayed (1..8)
     */
    void setScanLimit(int addr, int limit);

    /* 
     * Set the brightness of the display.
     * Params:
     * addr		the address of the display to control
     * intensity	the brightness of the display. (0..15)
     */
    void setIntensity(int addr, int intensity);

    /* 
     * Switch all Leds on the display off. 
     * Params:
     * addr	address of the display to control
     */
    void clearDisplay(int addr);

    /* 
     * Set the status of a single Led.
     * Params :
     * addr	address of the display 
     * row	the row of the Led (0..7)
     * col	the column of the Led (0..7)
     * state	If true the led is switched on, 
     *		if false it is switched off
     */
    void setLed(int addr, int row, int col, boolean state);

    /* 
     * Set all 8 Led's in a row to a new state
     * Params:
     * addr	address of the display
     * row	row which is to be set (0..7)
     * value	each bit set to 1 will light up the
     *		corresponding Led.
     */
    void setRow(int addr, int row, byte value);

    /* 
     * Set all 8 Led's in a column to a new state
     * Params:
     * addr	address of the display
     * col	column which is to be set (0..7)
     * value	each bit set to 1 will light up the
     *		corresponding Led.
     */
    void setColumn(int addr, int col, byte value);

 };

#endif	//MAX7219.h



