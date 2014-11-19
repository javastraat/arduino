/*
  MAX7219.h - A library for controling Leds with a MAX7219
*/

#include "MAX7219.h"

//the opcodes for the MAX7219
#define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

MAX7219::MAX7219(int dataPin, int clkPin, int csPin, int numDevices) {
    SPI_MOSI=dataPin;
    SPI_CLK=clkPin;
    SPI_CS=csPin;
    if(numDevices<=0 || numDevices>2 )
	numDevices=2;
    maxDevices=numDevices;
    pinMode(SPI_MOSI,OUTPUT);
    pinMode(SPI_CLK,OUTPUT);
    pinMode(SPI_CS,OUTPUT);
    digitalWrite(SPI_CS,HIGH);
    SPI_MOSI=dataPin;
    for(int i=0;i<16;i++) 
	status[i]=0x00;
    for(int i=0;i<maxDevices;i++) {
	spiTransfer(i,OP_DISPLAYTEST,0);
	//scanlimit is set to max on startup
	setScanLimit(i,7);
	//decode is done in source
	spiTransfer(i,OP_DECODEMODE,0);
	clearDisplay(i);
	//we go into shutdown-mode on startup
	shutdown(i,true);
    }
}

int MAX7219::getDeviceCount() {
    return maxDevices;
}

void MAX7219::shutdown(int addr, bool b) {
    if(addr<0 || addr>=maxDevices)
	return;
    if(b)
	spiTransfer(addr, OP_SHUTDOWN,0);
    else
	spiTransfer(addr, OP_SHUTDOWN,1);
}
	
void MAX7219::setScanLimit(int addr, int limit) {
    if(addr<0 || addr>=maxDevices)
	return;
    if(limit>=0 || limit<8)
    	spiTransfer(addr, OP_SCANLIMIT,limit);
}

void MAX7219::setIntensity(int addr, int intensity) {
    if(addr<0 || addr>=maxDevices)
	return;
    if(intensity>=0 || intensity<16)	
	spiTransfer(addr, OP_INTENSITY,intensity);
    
}

void MAX7219::clearDisplay(int addr) {
    int offset;

    if(addr<0 || addr>=maxDevices)
	return;
    offset=addr*8;
    for(int i=0;i<8;i++) {
	status[offset+i]=0;
	spiTransfer(addr, i+1,status[offset+i]);
    }
}

void MAX7219::setLed(int addr, int row, int column, boolean state) {
    int offset;
    byte val=0x00;

    if(addr<0 || addr>=maxDevices)
	return;
    if(row<0 || row>7 || column<0 || column>7)
	return;
    offset=addr*8;
    val=B10000000 >> column;
    if(state)
	status[offset+row]=status[offset+row]|val;
    else {
	val=~val;
	status[offset+row]=status[offset+row]&val;
    }
    spiTransfer(addr, row+1,status[offset+row]);
}
	
void MAX7219::setRow(int addr, int row, byte value) {
    int offset;
    if(addr<0 || addr>=maxDevices)
	return;
    if(row<0 || row>7)
	return;
    offset=addr*8;
    status[offset+row]=value;
    spiTransfer(addr, row+1,status[offset+row]);
}
    
void MAX7219::setColumn(int addr, int col, byte value) {
    byte val;

    if(addr<0 || addr>=maxDevices)
	return;
    if(col<0 || col>7) 
	return;
    for(int row=0;row<8;row++) {
	val=value >> (7-row);
	val=val & 0x01;
	setLed(addr,row,col,val);
    }
}

void MAX7219::spiTransfer(int addr, volatile byte opcode, volatile byte data) {
    //Create an array with the data to shift out
    int offset=addr*2;
    int maxbytes=maxDevices*2;

    for(int i=0;i<maxbytes;i++)
	spidata[i]=(byte)0;
    //put our device data into the array
    spidata[offset+1]=opcode;
    spidata[offset]=data;
    //enable the line 
    digitalWrite(SPI_CS,LOW);
    //Now shift out the data 
    for(int i=maxbytes;i>0;i--)
 	shiftOut(SPI_MOSI,SPI_CLK,MSBFIRST,spidata[i-1]);
    //latch the data onto the display
    digitalWrite(SPI_CS,HIGH);
}    


