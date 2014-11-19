
#include <Wire.h>

#include <string.h> //needed for strlen()

#include <inttypes.h>

#include <Arduino.h>  //all things wiring / arduino
  
#include "LCDi2cW.h"
  

#define _LCDEXPANDED				// If defined turn on advanced functions

// Global Vars 

uint8_t g_num_lines = 4;
uint8_t g_num_col = 20;
int g_i2caddress = 0x4C;
uint8_t g_display = 0;
int g_cmdDelay = 50;
int g_charDelay = 2;



// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	LCDi2c Class
// []
// []	num_lines = 1-4
// []   num_col = 1-80
// []   i2c_address = 7 bit address of the device
// []   display = Not used at this time
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
//--------------------------------------------------------



LCDi2cW::LCDi2cW (uint8_t num_lines,uint8_t num_col,int i2c_address,uint8_t display){
	
	g_num_lines = num_lines;
	g_num_col = num_col;
	g_i2caddress = i2c_address;
	g_display = display;	
}

// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	initiatize lcd after a short pause
// []
// []	Init the i2c buss
// []   Put the display in some kind of known mode
// []   Put the cursor at 0,0
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


void LCDi2cW::init () {
	
	Wire.begin();
	on();
	clear();
	blink_off();
	cursor_off(); 
	home();
	
}

// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Over ride the default delays used to send commands to the display
// []
// []	The default values are set by the library
// []   this allows the programer to take into account code delays
// []   and speed things up.
// []   
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


void LCDi2cW::setDelay (int cmdDelay,int charDelay) {
	
	g_cmdDelay = cmdDelay;
	g_charDelay = charDelay;
	
}




// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []   Send a command to the display. 
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


void LCDi2cW::command(uint8_t value) {

  Wire.beginTransmission(g_i2caddress);
  Wire.write(0xFE);
  Wire.write(value);
  Wire.endTransmission(1);
  delay(g_cmdDelay);
}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []   Send a command to the display. 
// []
// []	This is also used by the print, and println
// []	
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]



size_t LCDi2cW::write(uint8_t value) {

  Wire.beginTransmission(g_i2caddress);
  Wire.write(value);
  Wire.endTransmission(1);
  delay(g_charDelay);

}



// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Clear the display, and put cursor at 0,0 
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


void LCDi2cW::clear(){
	
      command(0x14);
 
}

// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Home to custor to 0,0
// []
// []	Do not disturb data on the displayClear the display
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


void LCDi2cW::home(){

	setCursor(0,0);					// The command to home the cursor does not work on the version 1.3 of the display
}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Turn on the display
// []
// []	Depending on the display, might just turn backlighting on
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

void LCDi2cW::on(){

      command(0x0A);

}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Turn off the display
// []
// []	Depending on the display, might just turn backlighting off
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

void LCDi2cW::off(){

      command(0x0B);

        
}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Turn on the underline cursor
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

void LCDi2cW::cursor_on(){


      command(0x0E);

        
}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Turn off the underline cursor
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

void LCDi2cW::cursor_off(){

      command(0x0F);
}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Turn on the blinking block cursor
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


void LCDi2cW::blink_on(){

      command(0x12);

}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Turn off the blinking block cursor
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

void LCDi2cW::blink_off(){

      command(0x13);
 
}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Position the cursor to position line,column
// []
// []	line is 0 - Max Display lines
// []	column 0 - Max Display Width
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


void LCDi2cW::setCursor(uint8_t line_num, uint8_t x){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(0xFE);
      Wire.write(0x0C);
      Wire.write(line_num);
      Wire.write(x);
      Wire.endTransmission(1);
      delay(g_cmdDelay*2);

}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Return the status of the display
// []
// []	Does nothing on this display
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]	

uint8_t LCDi2cW::status(){
	
	return 0;
}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Read data from keypad
// []
// []	
// []	
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


uint8_t LCDi2cW::keypad (){

  uint8_t data = 0;

  //  Send Keypad read command
  Wire.beginTransmission(g_i2caddress);
  Wire.write(0xFE);
  Wire.write(0x1B);
  Wire.endTransmission(1);
  delay(g_cmdDelay);
  
  //  Connect to device and request byte
  Wire.beginTransmission(g_i2caddress);
  Wire.requestFrom(g_i2caddress, 1);
	
  if (Wire.available()) {
    data = Wire.read();
  }

return data;
	
}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Initialize type of graph
// []
// [] LCDI2C_VERTICAL_BAR_GRAPH    1
// [] LCDI2C_HORIZONTAL_BAR_GRAPH  2
// [] LCDI2C_HORIZONTAL_LINE_GRAPH 3
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]



uint8_t LCDi2cW::init_bargraph(uint8_t graphtype)
{
	switch (graphtype)
		{
		case LCDI2C_VERTICAL_BAR_GRAPH:
				Wire.beginTransmission(g_i2caddress);
				Wire.write(0xFE);
				Wire.write(0x18);
				Wire.endTransmission(1);
				break;
		case LCDI2C_HORIZONTAL_BAR_GRAPH:
				Wire.beginTransmission(g_i2caddress);
				Wire.write(0xFE);
				Wire.write(0x16);
				Wire.write(0x00);
				Wire.endTransmission(1);
				break;
		case LCDI2C_HORIZONTAL_LINE_GRAPH:
				Wire.beginTransmission(g_i2caddress);
				Wire.write(0xFE);
				Wire.write(0x16);
				Wire.write(0x01);
				Wire.endTransmission(1);
				break;
		default:
				return 1;
		}
	delay(g_cmdDelay);
	return 0;
}

// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Draw Horizontal Graph
// []
// [] Row to draw on
// [] Column to start
// [] Length of graph
// [] Pixel col end
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


void LCDi2cW::draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end)
{
	Wire.beginTransmission(g_i2caddress);
	Wire.write(0xFE);
	Wire.write(0x17);
	Wire.write(row);
	Wire.write(column);
	Wire.write(len);
	Wire.write(pixel_col_end);
	Wire.endTransmission(1);
	delay(g_cmdDelay);
}

// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Draw Vertical Graph
// []
// [] Row to draw on
// [] Column to start
// [] Length of graph
// [] Pixel col end
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

void LCDi2cW::draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_row_end)
{
	Wire.beginTransmission(g_i2caddress);
	Wire.write(0xFE);
	Wire.write(0x19);
	Wire.write(row);
	Wire.write(column);
	Wire.write(len);
	Wire.write(pixel_row_end);
	Wire.endTransmission(1);
	delay(g_cmdDelay);
}




// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Load data for a custom character
// []
// []	Char = custom character number 0-7
// []	Row is array of chars containing bytes 0-7
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


void LCDi2cW::load_custom_character(uint8_t char_num, uint8_t *rows)
{


	Wire.beginTransmission(g_i2caddress);
	Wire.write(0xFE);
	Wire.write(0x1A);
	Wire.write(char_num);
	for (uint8_t i = 0 ; i < 8 ; i++)
		Wire.write(rows[i]);
	Wire.endTransmission(1);
	delay(g_cmdDelay);
}




void LCDi2cW::setBacklight(uint8_t new_val)
{
	
	Wire.beginTransmission(g_i2caddress);
	Wire.write(0xFE);
	Wire.write(0x03);
	Wire.write(new_val);
	Wire.endTransmission(1);
	delay(g_cmdDelay);

}


void LCDi2cW::setContrast(uint8_t new_val)
{
	
	Wire.beginTransmission(g_i2caddress);
	Wire.write(0xFE);
	Wire.write(0x04);
	Wire.write(new_val);
	Wire.endTransmission(1);
	delay(g_cmdDelay);
}


void  LCDi2cW::printstr(const char c[])
{
	byte len;

	while (*c)
		{
		len = min(strlen(c), g_num_col);
		Wire.beginTransmission(g_i2caddress);
		Wire.write(0xFE);
		Wire.write(0x15);
		Wire.write(len);
		while (len--)
			Wire.write(*c++);
		Wire.endTransmission(1);
		if (*c)
			delay(g_charDelay);	// More to write.  Wait a bit
		}
}

