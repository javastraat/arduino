#ifndef LCDi2cW_h
#define LCDi2cW_h


#define _LCDEXPANDED				// If defined turn on advanced functions

#include <inttypes.h>

#include "Print.h"


class LCDi2cW : public Print {
	
public: 
	
LCDi2cW(uint8_t num_lines, uint8_t num_col, int i2c_address, uint8_t display);
	
	void command(uint8_t value);
	
	void init();
	
	void setDelay(int,int);
	
	size_t write(uint8_t);
	
	void clear();
	
	void home();
	
	void on();
	
	void off();
	
	void cursor_on();
	
	void cursor_off();
	
	void blink_on();
	
	void blink_off();
	
	void setCursor(uint8_t Line, uint8_t Col );
	
	
	// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
	// []
	// []	Extended Functions
	// []
	// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
	
	
#ifdef _LCDEXPANDED		

	
	uint8_t status();
	
	void load_custom_character(uint8_t char_num, uint8_t *rows);
	
	uint8_t keypad();
	
	void printstr(const char[]);
	
	 // Values for graphtype in calls to init_bargraph
	
	 #define LCDI2C_VERTICAL_BAR_GRAPH    1
	 #define LCDI2C_HORIZONTAL_BAR_GRAPH  2
	 #define LCDI2C_HORIZONTAL_LINE_GRAPH 3
	 
	 uint8_t init_bargraph(uint8_t graphtype);
	
	 void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end);
	
	 void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end);
	 
	 void setBacklight(uint8_t new_val);
	
	 void setContrast(uint8_t new_val);
	 
		
#endif
	
private:
	
	
};

#endif

