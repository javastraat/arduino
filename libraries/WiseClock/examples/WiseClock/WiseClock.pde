/** 
 * WiseClock - displays text retrieved from I2C EEPROM, time from RTC;
 *           - display is either an 8x8 RG LED matrix or an 16x8 single color LED matrix
 *              i)  to select 16x8 single colour display, enable the line [#define _16x8_] below;
 *              ii) to select 8x8 RG display, disable (comment out) the line [#define _16x8_];
 *           - display is driven by 2x595s (for columns) and direct outputs (for rows);
 *           - display ISR (shiftOut the video mem) is executed on timer, 7812 times per second;
 * 
 * Reference:
 * http://tinkerlog.com (email: alex@tinkerlog.com)
 * http://www.arduino.cc/en/Tutorial/ShiftOut
 * http://www.uchobby.com/index.php/2007/11/24/arduino-interrupts/
 * 
 */

#include <WProgram.h>
#include <avr/sleep.h>
#include <EEPROM.h>  // internal eeprom
#include <Wire.h>
#include <DS1307.h>
//#include "Conway.h"


// define this when display is 16x8, mono color;
//#define _16x8_   true


// used to enable/disable Serial.print, time-consuming operation;
// to minimize the code size, all references to Serial should be commented out;
//#define _DEBUG_     true

/*
* TO DO:
*    - menu option to load text file into EEPROM (read on serial port from windows app);
*    - clock alarm; talks with X10 transmitter on serial port;
*    - wireless networking (interface with fonera on serial port); requires only code;
*    - audio spectrum analyzer; requires analog INPUT PIN;
*    - servo to control orientation of the display (when used as a light fixture in the ceiling); requires OUTPUT PIN;
*    - use a tilt sensor to display and scroll in the right direction; requires INPUT PIN;
*
* DONE:
*    (Mar 29/09) set clock by remote control, option 'S'; enter time as 4 digits, HHMM;
*    (Mar 29/09) sleep mode directly from "Power TV" button on the remote control;
*    (May 17/09) cover the single color 16x8 LED matrix (using same circuitry as RG LED matrix);
*    (Jun 03/09) disabled unused menu options; re-arranged menu options order and definitions;
*                reset display before sleep mode;
*    (Jun 20/09) set unused pins as outputs to save power (see http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1243213127/15#15);
*/



//-----------------------------------------------------------------------------
/* Pins used in Wise Clock are as follows:
	analog  0..2          - FREE to use (servo, buzzer, mux/demux select, tengu sound signal etc);
	analog  3 (INPUT)     – menu button;
	analog  4, 5          – I2C (RTC, eeprom);
	digital 0, 1          – serial (tx/rx)
	digital 2 (INPUT)     – IR receiver/ISR0;
	digital 3-13 (OUTPUT) – display;
*/
//-----------------------------------------------------------------------------



//--------------------------------------------------------------------------------------
/* DS1307 Real Time Clock - uses the wire library (I2C);
 *
 * RTC is set up through a separate sketch (+windows app talking on serial port).
 * I2C bus address of DS1307 is B1101000 (already defined in RTC.h).
 *
 * SETUP:     _ _                                
 *        X1-|oU |-Vcc
 *        X2-|   |-SQW/OUT square wave 1Hz output signal;
 *  Vbat(3V)-|   |-SCL     to Arduino analog 5
 *       GND-|   |-SDA     to Arduino analog 4
 *            --- 
 *
 * Pull-up resistors (10k) are required on SDA (pins 5), SCL (pin 6), and SQW (pin 7).
 * Pin 7 (SQW) of DS1307 is not used (not connected to arduino).
 *
 *
 * References:
 *    http://www.datasheetcatalog.org/datasheet/maxim/DS1307.pdf
*/
//--------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------
/* 24LC256 256K EEPROM - uses the Wire library (I2C);
 *
 * The EEPROM stores messages as text lines separated by CR/LF (13/10).
 * Its content is already loaded with text messages (separate sketch, receiving data on COM from windows app/script).
 *
 * Addresses are ints - 0000-7FFF (32767) Data is bytes (8 bits x 32767 = 256K)
 * Functions for R/W of single byte or a page of bytes. Max page is 28 bytes.
 *
 * SETUP:           _ _                                
 * Arduino GND- A0-|oU |-Vcc  to Arduino Vcc
 * Arduino GND- A1-|   |-WP   to GND for now. Set to Vcc for write protection.
 * Arduino GND- A2-|   |-SCL  to Arduino analog 5
 * Arduino GND-Vss-|   |-SDA  to Arduino analog 4
 *                  ---       (A2, A1, A0 to GND for 1010000 (0x50) address.)
 *                            If set to Vcc adds to address (1010,A2,A1,A0)
 *
*/
//--------------------------------------------------------------------------------------


// I2C Bus address of 24LC256 256K EEPROM;
// if more than one eeprom, they will have different addresses (h/w configured);
#define I2C_ID     0x50

// next writeable address in EEPROM (where the next message will be written);
// REM: must be initialized somehow (stored in local eprom?);
long crtWriteAddress;


// global address of the last byte read from eeprom;
// set to a random value in setup();
unsigned int crtReadAddress = 0;


//--------------------------------------------------------------------------------------
/*
 * A temperature sensor DS18B20 was initially set up on analog pin 2 (using 1-wire).
 * The code to handle it took 3K, not worth it for the info provided.
*/
//--------------------------------------------------------------------------------------



// analog input pin for tengu;
// REM: not wired yet; requires an extra shield (mic, amp, speaker);
#define TENGU_PIN         1

// push button on analog pin 3; used for menu selection (superseedes IR remote control);
#define MENU_BUTTON_PIN   3

// IR receiver (pin wired through 220 ohm resistor) on digital 2; serviced through ISR0;
#define IR_PIN            2


// pins used for LED matrix rows (multiplexed);
#define SHIFT_CLOCK_PIN   4
#define STORE_CLOCK_PIN   5
#define SER_DATA_PIN      6

// pins assigned to LED matrix columns;
byte pinForRow[8] = {8, 9, 10, 11, 12, 13, 7, 3};

//-----------------------------------------------------------------------------


// display colours (aka "pages");
#define BLACK   0
#define RED     1
#define GREEN   2
#define ORANGE  3


// menu options;
#define OPTION_BOTH_Q_T      0	// B (both quote and time)
#define OPTION_QUOTE         1	// Q
#define OPTION_TIME          2	// T
#define OPTION_SLEEP         3	// X
#define OPTION_SETTIME       4	// S (set time)

/*
#define OPTION_CONWAY        5	// C
#define OPTION_TENGU         6	// E; REM: disabled for now;
#define OPTION_DICE          7	// D; REM: disabled for now;
#define OPTION_SPEED         8	// F (set scrolling speed); REM: disabled for now;
#define OPTION_ALARM         9	// A (set alarm by user); REM: disabled for now;
#define OPTION_LOAD          10	// L (load message to EEPROM); REM: disabled for now;
*/

// maximum number of options in the menu; MUST be incremented when a new option is added!;
// REM: update when enable the other menu options;
#define MAX_OPTION           4

// set default option according to preference;
#define DEFAULT_OPTION       OPTION_QUOTE


// current menu option, a value between 0 and MAX_OPTION;
volatile int menuOption = DEFAULT_OPTION;

// letters displayed for each menu option;
char menuLetter[] = {'B', 'Q', 'T', 'X', 'S', 'C', 'E', 'D', 'F', 'A', 'L'};


// global counter used to track when to read the time and temperature;
byte nCounter = 0;


// buffer used for storing the time when setting it;
int timeDigit[4] = {0};
// incremented as digits representing the time (HHMM) are entered;
// time will be set in RTC when index reaches 4 (meaning all 4 digits have been entered);
int indexTimeDigit = 0;


// the instance of Conway's game of life;
#ifdef OPTION_CONWAY
  Conway ConwayGame = Conway();
#endif


// parameters for IR receiver;
//#define start_bit  2000     // start bit threshold (microseconds)
#define IR_BIN_1      1000    // binary 1 threshold  (microseconds)
#define IR_BIN_0       400    // binary 0 threshold  (microseconds)


// SONY codes for the remote control;
#define SONY_CH_PLUS         144
#define SONY_CH_MINUS        145
#define SONY_VOL_PLUS        146
#define SONY_VOL_MINUS       147
#define SONY_MUTE            148
#define SONY_POWER_TV        149
#define SONY_1               128
#define SONY_2               129
#define SONY_3               130
#define SONY_4               131
#define SONY_5               132
#define SONY_6               133
#define SONY_7               134
#define SONY_8               135
#define SONY_9               136
#define SONY_0               137
#define SONY_DISPLAY         186
#define SONY_ENTER           139
#define SONY_TV_VIDEO        165
#define SONY_TV              170
#define SONY_JUMP            187



// button debouncing adapted from http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210559123/7;
#define BOUNCE_TIME_BUTTON  600   // bounce time in ms for the menu button;
#define BOUNCE_TIME_IR      500   // bounce time in ms for the IR receiver;


// last time a command (IR or button) was received; used for debouncing;
volatile unsigned long menuTime = 0;

// indicates that one of the menu options (0..MAX_OPTION) is currently displayed;
boolean isMenuOptionOnDisplay = false;

// indicates that display needs to be refreshed;
boolean mustRefreshDisplay = false;


// when alternating between displaying quotes and time, start with the time;
boolean isDisplayingTime  = true;
boolean isDisplayingQuote = false;


// indicates if a new game (Conway's, dice etc) was requested;
boolean isNewGame = true;

// index of the tengu face, value between 0 and 6;
byte tenguStage = 0;

// used to calculate the scrolling speed; value from 0 to 7, read in OPTION_SPEED menu option;
byte level = 5;


// used for timing events, e.g. to set timeout in OPTION_SPEED menu option;
volatile unsigned long eventTime = 0;



// interrupt handler for the infrared receiver (digital pin 2);
// it ignores codes received faster then the bounce time;
// only some commands determine the menu to be displayed;
void irReceiverISR()
{
  int key = getIRKey();
  if (key > 0)
  {
     switch (key)
     {
       case SONY_POWER_TV:
          menuOption = OPTION_SLEEP;
          menuTime = 0;    // display no menu;
          break;

       // used for selecting next up menu option;
       case SONY_CH_PLUS:
          menuOption++;
          if (menuOption > MAX_OPTION) menuOption = 0;
          mustRefreshDisplay = true;
          break;
          
       // used for selecting next down menu option;
       case SONY_CH_MINUS:
          menuOption--;
          if (menuOption < 0) menuOption = MAX_OPTION;
          mustRefreshDisplay = true;
          break;
          
       // used for adjusting the scrolling speed;
       case SONY_VOL_PLUS:
          level++;
          if (level > 7) level=0;
#ifdef OPTION_SPEED
          if (menuOption == OPTION_SPEED)
          {
            showAdjustingLevel();
          }
          else
          {
            menuTime = 0;  // display no menu;
          }
#endif
          break;

       // used for adjusting the scrolling speed;
       case SONY_VOL_MINUS:
          level--;
          if (level < 0) level=7;
#ifdef OPTION_SPEED
          if (menuOption == OPTION_SPEED)
          {
            showAdjustingLevel();
          }
          else
          {
            menuTime = 0;  // display no menu;
          }
#endif
          break;

       // used for starting new game (Conway's, dice etc);
       case SONY_ENTER:
          isNewGame = true;
          menuTime = 0;  // display no menu;
#ifdef OPTION_DICE
          if (menuOption == OPTION_DICE)   showDice();
#endif
          break;

       // used for setting up the time;
       case SONY_1:
       case SONY_2:
       case SONY_3:
       case SONY_4:
       case SONY_5:
       case SONY_6:
       case SONY_7:
       case SONY_8:
       case SONY_9:
       case SONY_0:
          if (menuOption == OPTION_SETTIME)
          {
            timeDigit[indexTimeDigit] = (key - SONY_1 + 1) % 10;

            // display the just-entered digit;
#ifdef _16x8_
            setScreenMem_16x8('0' + timeDigit[indexTimeDigit]);
#else
            setScreenMem(RED, '0' + timeDigit[indexTimeDigit]);
#endif
            indexTimeDigit++;
          }
          break;
     }
  }
}


// executed as a result of the menu button being pressed;
// determines the menu to be displayed;
void processMenuButton()
{
  // debouncing;
  if (abs(millis() - menuTime) < BOUNCE_TIME_BUTTON)
    return;

  menuTime = millis();

  menuOption++;
  if (menuOption > MAX_OPTION) menuOption = 0;
  mustRefreshDisplay = true;    
}


byte soft_prescaler = 0;

byte activeRow = 0;

// REM: should be one or the other, depending on _16x8_;
// video memory for the 16x8 display;
unsigned int screenMem_16x8[8] = {0};

// video memory for the 8x8 RG display;
byte screenMem[16] = {0};


// buffer for time display;
char timeBuffer[]   = " 12:45    ";
byte timeBufferPtr  = 0;
byte timeBufferSize = strlen(timeBuffer);


//----------------------------------------------------------------------------
// The displayed messages are read from eprom (24LC256).
// The eprom is already loaded with message lines, separated by CR (13).
// In order to be displayed, each line is first loaded from eprom into buffer "msgLine".
// After the line is displayed completely (on the scrolling display), a new line is loaded (read) from eprom.

byte lastReadByte = 0;

// current line string is built by copying each character to the position of the pointer, then advancimng the pointer;
char msgLine[200] = {0};
char* msgLinePtr = &msgLine[0];

int msgLineSize;	// size of the current string; set after line is read from eprom;
int msgLineIndex  = 0;
//----------------------------------------------------------------------------

// (fragments of) these 2 characters are on the screen at any moment;
byte char1 = 0;
byte char2 = 0;
// used for the 16x8 display;
byte char3 = 0;
byte char4 = 0;



// current colour used for display;
byte page = GREEN;



void setup()
{
  // Calculation for timer 2
  // 16 MHz / 8 = 2 MHz (prescaler 8)
  // 2 MHz / 256 = 7812 Hz
  // soft_prescaler = 15 ==> 520.8 updates per second
  // 520.8 / 8 rows ==> 65.1 Hz for the complete display
  TCCR2A = 0;           // normal operation
  TCCR2B = (1<<CS21);   // prescaler 8
  TIMSK2 = (1<<TOIE2);  // enable overflow interrupt


  // define outputs for serial shift registers
  pinMode(SHIFT_CLOCK_PIN, OUTPUT);
  pinMode(STORE_CLOCK_PIN, OUTPUT);
  pinMode(SER_DATA_PIN,    OUTPUT);

  // set outputs for the 8 matrix rows;
  for (int i=0; i<8; i++)
    pinMode(pinForRow[i], OUTPUT);

  // (Jun 20/09) set unused pins as outputs to save power;
  pinMode(14, OUTPUT);	// analog 0;
  pinMode(15, OUTPUT);  // analog 1;
  pinMode(16, OUTPUT);  // analog 2;


  Wire.begin();         // join I2C bus (address optional for master);

  // start with a random (seeded by the clock) quotation;
  updateTimeBuffer();
  randomSeed(timeBuffer[5]);
  crtReadAddress = random(0, 10000);
  fetchLineFromEprom();

  resetDisplay();

#ifdef _DEBUG_
    Serial.begin(9600);
#endif

  // define ISR for pin 2 (IR receiver); used for displaying the menu;
  attachInterrupt(0, irReceiverISR, LOW);
}


/**
 * ISR TIMER2_OVF_vect
 * Gets called 7812 times per second. Used for display.
 */
ISR(TIMER2_OVF_vect)
{
  soft_prescaler++;
  if (soft_prescaler == 15)
  {
    // display the next row
#ifdef _16x8_
    displayActiveRow_16x8();
#else
    displayActiveRow();
#endif
    soft_prescaler = 0;
  }
}


/**
 * displayActiveRow
 * The row is active low. But we are using a ULN2803, that is inverting. 
 * So we have to use high to switch a row on.
 */
void displayActiveRow()
{
  // disable current row;
  digitalWrite(pinForRow[activeRow], LOW);

  // set next row;
  activeRow = (activeRow+1) % 8;

  // shift out values for this row;
  shiftOutRow(screenMem[activeRow], screenMem[8+activeRow]);
  
  // switch to new row;
  digitalWrite(pinForRow[activeRow], HIGH);
}


void displayActiveRow_16x8()
{
  // disable current row;
  digitalWrite(pinForRow[activeRow], LOW);

  // set next row;
  activeRow = (activeRow+1) % 8;

  // shift out values for this row;
  byte highByte = screenMem_16x8[activeRow] >> 8;
  byte lowByte  = screenMem_16x8[activeRow] & 0xFF;
  shiftOutRow(highByte, lowByte);
  
  // switch to new row;
  digitalWrite(pinForRow[activeRow], HIGH);
}


void shiftOutRow(byte red, byte green)
{
  digitalWrite(STORE_CLOCK_PIN, LOW);
  shiftOut(SER_DATA_PIN, SHIFT_CLOCK_PIN, LSBFIRST, red);   
  shiftOut(SER_DATA_PIN, SHIFT_CLOCK_PIN, LSBFIRST, green);   
  // return the latch pin high to signal chip that it 
  // no longer needs to listen for information
  digitalWrite(STORE_CLOCK_PIN, HIGH);
}


void resetDisplay()
{
#ifdef _16x8_
  for (byte i = 0; i < 8; i++)  screenMem_16x8[i] = 0x00;
  char1 = 0;
  char2 = 0;
  char3 = 0;
  char4 = 0;
#else
  for (byte i = 0; i < 16; i++)  screenMem[i] = 0x00;
  char1 = 0;
  char2 = 0;
#endif

  // reset the buffer pointers;
  msgLineIndex  = 0;
  timeBufferPtr = 0;
}


// added Feb14/09 to delete the display by moving a dot from
// the upper left corner to the lower right corner;
void resetDisplayByMovingDot()
{
  byte mask;

  // the dot has the color of the current page;
  // REM: will not work for orange (both pages would need to be masked dot by dot);
  byte idx = ((page & GREEN) == GREEN)? 8 : 0;

  for (byte y=0; y < 8; y++)
  {
    for (byte x = 0; x < 8; x++)
    {
      // show dot at the particular location;
      mask = 1<<(7-x);
      screenMem[y+idx] = screenMem[y+idx] | mask;

      delay(40);

      // delete dot from the particular location, leaving location empty;
      mask = 0x7F >> x;
      screenMem[y+idx] = screenMem[y+idx] & mask;
    }
  }

  char1 = 0;
  char2 = 0;

  // reset the buffer pointers;
  msgLineIndex  = 0;
  timeBufferPtr = 0;
}


// statically displays the given sprite;
void setScreenMem(byte color, byte sprite[8])
{
  page = color;  // set the current page (color);
  
  byte row;
  for (byte i = 0; i < 8; i++)
  {
    row = sprite[i];
    if ((color & RED)   == RED)    screenMem[i]   = row;
    if ((color & GREEN) == GREEN)  screenMem[i+8] = row;
  }
}


// statically displays the given sprite in the middle of the 16x8 screen;
void setScreenMem_16x8(byte sprite[8])
{
  for (byte i = 0; i < 8; i++)
  {
    screenMem_16x8[i] = ((unsigned int) sprite[i]) << 4;
  }
}


// statically displays the specified character;
void setScreenMem(byte color, char charToDisplay)
{
  page = color;  // set the current page (color);

  byte row;
  for (byte i = 0; i < 8; i++)
  {
    int addrEeprom = (charToDisplay-32) * 8 + i;
    row = EEPROM.read(addrEeprom);
    if ((color & RED)   == RED)    screenMem[i]   = row;
    if ((color & GREEN) == GREEN)  screenMem[i+8] = row;
  }
}


// statically displays the specified character in the middle of the 16x8 display;
void setScreenMem_16x8(char charToDisplay)
{
  for (byte i = 0; i < 8; i++)
  {
    int addrEeprom = (charToDisplay-32) * 8 + i;
    byte line = EEPROM.read(addrEeprom);
    screenMem_16x8[i] = ((unsigned int) line) << 4;
  }
}


// at any given time, the display contains 2 fragments of characters, as they are scrolled from right to left;
void setScreenMem(byte sprite1[8], byte sprite2[8])
{
  byte row, rowChar1, rowChar2;

  // required here because level adjustment should have been an atomic operation, that is, 
  // [level++; level = level % 7] in IR ISR should have been executed together; they sometimes are not;
  if (level > 7) level=0;
  if (level < 0) level=7;

  // adjusts the scrolling speed;
  byte wait = ((8-level) << 5) - 1;

  // scroll 5 times to the left;
  for (byte x=1; x<=5; x++)
  {
    // for each row;
    for (char i = 0; i < 8; i++)
    {
      rowChar1 = sprite1[i];
      rowChar2 = sprite2[i];

      row = ((rowChar1 << x) + (rowChar2 >> (5-x)));

      if ((page & RED)   == RED)     screenMem[i]   = row;
      if ((page & GREEN) == GREEN)   screenMem[i+8] = row;
    }
    delay(wait);
  }
}


void setScreenMem_16x8(byte sprite1[8], byte sprite2[8], byte sprite3[8], byte sprite4[8])
{
  // REM: this piece of code should be made into a function;
  // required here because level adjustment should have been an atomic operation, that is, 
  // [level++; level = level % 7] in IR ISR should have been executed together; they sometimes are not;
  if (level > 7) level=0;
  if (level < 0) level=7;

  // adjusts the scrolling speed;
  byte wait = ((8-level) << 5) - 1;


  unsigned long row[16] = {0};
  
  // for each row;
  for (byte i = 0; i < 8; i++)
  {
      byte c1 = sprite1[i] >> 1;
      byte c2 = sprite2[i] >> 1;
      byte c3 = sprite3[i] >> 1;
      byte c4 = sprite4[i] >> 1;
      row[i] = ((((((unsigned long) c1 << 5) + c2) << 5) + c3) << 5) + c4;
  }  
  
  // scroll 5 times to the left (5 being the width of a char, as defined);
  for (byte x = 1; x <= 5; x++)
  {
    // for each row;
    for (byte i = 0; i < 8; i++)
    {
      screenMem_16x8[i] = row[i] >> (5-x);
    }
    delay(wait);
  }
}



void loop()
{
  // check menu button (connects analog pin 3 to ground);
  int val = analogRead(MENU_BUTTON_PIN);
  if (val < 5)
  {
    // menu button was pressed;
    processMenuButton();
  }
  
  
  // display the menu option for 5 seconds after menu button was pressed;
  if ((menuTime > 0) && (millis() - menuTime < 5000))
  {
      isMenuOptionOnDisplay = true;

      if (mustRefreshDisplay)
      {
        resetDisplay();
        // display menu option;
        // used to be a digit; switched to letter (more intuitive + more menu options available);
#ifdef _16x8_
        setScreenMem_16x8(menuLetter[menuOption]);
#else
        setScreenMem(GREEN, menuLetter[menuOption]);
#endif
        mustRefreshDisplay = false;
      }
  }
  else
  {
    // clear screen after displaying menu option for 5 seconds;
    if (isMenuOptionOnDisplay)
    {
      resetDisplayByMovingDot();
      isMenuOptionOnDisplay = false;

#ifdef OPTION_CONWAY
      if (menuOption == OPTION_CONWAY) isNewGame = true;
#endif
    }

    bigOptionSwitch();    
  }
}


// function called for menu option OPTION_DICE;
void showDice()
{
  if (!isNewGame)
    return;
    
  // generate random number between 0 and 5;
  randomSeed(millis());
  byte dice = random(0, 6);
  
  byte dicePattern[6][8] = {
      {
        0x00,    // ________
        0x00,    // ________
        0x00,    // ________
        0x18,    // ___XX___
        0x18,    // ___XX___
        0x00,    // ________
        0x00,    // ________
        0x00,    // ________
      },
      {
        0x00,    // ________
        0x18,    // ___XX___
        0x18,    // ___XX___
        0x00,    // ________
        0x00,    // ________
        0x18,    // ___XX___
        0x18,    // ___XX___
        0x00,    // ________
      },
      {
        0xC0,    // XX______
        0xC0,    // XX______
        0x00,    // ________
        0x18,    // ___XX___
        0x18,    // ___XX___
        0x00,    // ________
        0x03,    // ______XX
        0x03,    // ______XX
      },
      {
        0x00,    // ________
        0x66,    // _XX__XX_
        0x66,    // _XX__XX_
        0x00,    // ________
        0x00,    // ________
        0x66,    // _XX__XX_
        0x66,    // _XX__XX_
        0x00,    // ________
      },
      {
        0x66,    // _XX__XX_
        0x66,    // _XX__XX_
        0x00,    // ________
        0x18,    // ___XX___
        0x18,    // ___XX___
        0x00,    // ________
        0x66,    // _XX__XX_
        0x66,    // _XX__XX_
      },
      {
        0x66,    // _XX__XX_
        0x66,    // _XX__XX_
        0x00,    // ________
        0x66,    // _XX__XX_
        0x66,    // _XX__XX_
        0x00,    // ________
        0x66,    // _XX__XX_
        0x66,    // _XX__XX_
      }
  };
  
  // display dice pattern;
  setScreenMem(ORANGE, dicePattern[dice]);

  isNewGame = false;
}


// function called for menu option OPTION_TENGU;
void showTengu()
{
  // read tengu index (value between 0 and 6);
  // tenguStage = analogRead(TENGU_PIN);

  byte tengu[][8] =
  {
   {
    0x42,    // _X____X_  tengu 0
    0x00,    // ________
    0x18,    // ___XX___
    0x00,    // ________
    0x00,    // ________
    0x18,    // ___XX___
    0x18,    // ___XX___
    0x00,    // ________
  },
  {
    0x42,    // _X____X_  tengu 1
    0x00,    // ________
    0x18,    // ___XX___
    0x00,    // ________
    0x00,    // ________
    0x3C,    // __XXXX__
    0x3C,    // __XXXX__
    0x00,    // ________
  },
  {
    0x42,    // _X____X_  tengu 2
    0x00,    // ________
    0x18,    // ___XX___
    0x00,    // ________
    0x18,    // ___XX___
    0x24,    // __X__X__
    0x24,    // __X__X__
    0x18,    // ___XX___
  },
  {
    0x42,    // _X____X_  tengu 3
    0x00,    // ________
    0x18,    // ___XX___
    0x00,    // ________
    0x3C,    // __XXXX__
    0x24,    // __X__X__
    0x24,    // __X__X__
    0x3C,    // __XXXX__
  },
  {
    0x42,    // _X____X_  tengu 4
    0x00,    // ________
    0x18,    // ___XX___
    0x00,    // ________
    0x7E,    // _XXXXXX_
    0x42,    // _X____X_
    0x42,    // _X____X_
    0x7E,    // _XXXXXX__
  },
  {
    0x42,    // _X____X_  tengu 5
    0x00,    // ________
    0x18,    // ___XX___
    0x00,    // ________
    0x7E,    // _XXXXXX_
    0x81,    // X______X
    0x81,    // X______X
    0x7E,    // _XXXXXX_
  },
  {
    0x42,    // _X____X_  tengu 6
    0x00,    // ________
    0x18,    // ___XX___
    0x7E,    // _XXXXXX_
    0x81,    // X______X
    0x81,    // X______X
    0x81,    // X______X
    0x7E,    // _XXXXXX_
  }
 };

  tenguStage = (tenguStage + 1) % 7;

  setScreenMem(ORANGE, tengu[tenguStage]);
}


// display as ascending histogram (8 bars with heights from 1 to 8);
void showAdjustingLevel()
{
//  resetDisplay();

  byte histogram[8] =
  {
    0x01,    // _______X
    0x03,    // ______XX
    0x07,    // _____XXX
    0x0F,    // ____XXXX
    0x1F,    // ___XXXXX
    0x3F,    // __XXXXXX
    0x7F,    // _XXXXXXX
    0xFF,    // XXXXXXXX
  };

  setScreenMem(GREEN, histogram);

  byte verticalBar[8];
  byte offset = 7 - level;
  for (byte i=0; i<8; i++)
  {
    verticalBar[i] = (i<offset) ? 0 : (1 << offset);
  }
  
  setScreenMem(RED, verticalBar);
}


void updateTimeBuffer()
{
  int rtc[7];
  RTC.get(rtc, true);

  int second = rtc[0];
  int minute = rtc[1];
  int hour   = rtc[2];
  int day    = rtc[4];
  int month  = rtc[5];
  int year   = rtc[6];

#ifdef _DEBUG_
    Serial.print("Time is ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.println("");
#endif

  // build the string containing formatted time;
  timeBuffer[1] = (hour < 10) ? ' ' : ('0' + hour/10);
  timeBuffer[2] = '0' + hour%10;
  timeBuffer[4] = '0' + minute/10;
  timeBuffer[5] = '0' + minute%10;
}


// ignores codes received faster then the bounce time;
// returns negative (-1, -2, -3) if infrared reception error or when "debouncing";
// returns IR command (positive int) when code is read successfully;
int getIRKey()
{
  // "debouncing"; ignore IR command if coming too fast;
  if (abs(millis() - menuTime) < BOUNCE_TIME_IR)
    return -1;

  // wait for a start bit;
  if (pulseIn(IR_PIN, LOW, 200) < 2200)
    return -2;

  int data[12];
  // least significant bit first;
  data[0]  = pulseIn(IR_PIN, LOW);	// bits are only on low pulses;
  data[1]  = pulseIn(IR_PIN, LOW);
  data[2]  = pulseIn(IR_PIN, LOW);
  data[3]  = pulseIn(IR_PIN, LOW);
  data[4]  = pulseIn(IR_PIN, LOW);
  data[5]  = pulseIn(IR_PIN, LOW);
  data[6]  = pulseIn(IR_PIN, LOW);
  data[7]  = pulseIn(IR_PIN, LOW);
  data[8]  = pulseIn(IR_PIN, LOW);
  data[9]  = pulseIn(IR_PIN, LOW);
  data[10] = pulseIn(IR_PIN, LOW);
  data[11] = pulseIn(IR_PIN, LOW);
  // most significant bit last;

  int result = 0;
  int exponent = 1;
  
  for (int i=0; i<11; i++)
  {
    if (data[i] > IR_BIN_1)
    {
	data[i] = 1;    // bit is 1;
        result += exponent;
    }
    else
    {
	if (data[i] > IR_BIN_0)
        {
	  data[i] = 0;  // bit is 0;
	}
        else
        {
	  return -3;	// error;
	}
     }
     exponent = exponent << 1;
  }

  menuTime = millis();

  return result;
}


void displayAndScroll(char crtChar)
{
  // replace undisplayable characters with blank;
  if (crtChar < 32 || crtChar > 126)  crtChar = ' ';

  // indexes in the character bitmap definition table;
  char1 = char2;
  char2 = crtChar - 32;

  byte sprite1[8];
  byte sprite2[8];
  for (int i=0; i<8; i++)
  {
    sprite1[i] = EEPROM.read(char1 * 8 +i);
    sprite2[i] = EEPROM.read(char2 * 8 +i);
  }

  setScreenMem(sprite1, sprite2);
}


void displayAndScroll_16x8(char crtChar)
{
  // replace undisplayable characters with blank;
  if (crtChar < 32 || crtChar > 126)  crtChar = ' ';

  // indexes in the character bitmap definition table;
  char1 = char2;
  char2 = char3;
  char3 = char4;
  char4 = crtChar - 32;

  byte sprite1[8];
  byte sprite2[8];
  byte sprite3[8];
  byte sprite4[8];
  for (int i=0; i<8; i++)
  {
    sprite1[i] = EEPROM.read(char1 * 8 +i);
    sprite2[i] = EEPROM.read(char2 * 8 +i);
    sprite3[i] = EEPROM.read(char3 * 8 +i);
    sprite4[i] = EEPROM.read(char4 * 8 +i);
  }

  setScreenMem_16x8(sprite1, sprite2, sprite3, sprite4);
}


void rotateRight(byte s[8])
{
//byte n[0] = s[7] & 0x80      + (s[6] & 0x80) >> 1 + (s[5] & 0x80) >> 2 + (s[4] & 0x80) >> 3 + (s[3] & 0x80) >> 4 + (s[2] & 0x80) >> 5 + (s[1] & 0x80) >> 6 + (s[0] & 0x80) >> 7;
//byte n[1] = s[7] & 0x40 << 1 + (s[6] & 0x40)      + (s[5] & 0x40) >> 1 + (s[4] & 0x40) >> 2 + (s[3] & 0x40) >> 3 + (s[2] & 0x40) >> 4 + (s[1] & 0x40) >> 5 + (s[0] & 0x40) >> 6;
//byte n[2] = s[7] & 0x20 << 2 + (s[6] & 0x20) << 1 + (s[5] & 0x20)      + (s[4] & 0x20) >> 1 + (s[3] & 0x20) >> 2 + (s[2] & 0x20) >> 3 + (s[1] & 0x20) >> 4 + (s[0] & 0x20) >> 5;
//byte n[3] = s[7] & 0x10 << 3 + (s[6] & 0x10) << 2 + (s[5] & 0x10) << 1 + (s[4] & 0x10)      + (s[3] & 0x10) >> 1 + (s[2] & 0x10) >> 2 + (s[1] & 0x10) >> 3 + (s[0] & 0x10) >> 4;

  byte n[8];
  for (byte i=0; i<8; i++)
  {
    byte mask = 1 << (7-i);
    n[i] = (s[7] & mask) <<  i    +
           (s[6] & mask) << (i-1) +
           (s[5] & mask) << (i-2) +
           (s[4] & mask) << (i-3) +
           (s[3] & mask) << (i-4) +
           (s[2] & mask) << (i-5) +
           (s[1] & mask) << (i-6) +
           (s[0] & mask) << (i-7);
  }
}


void rotateLeft(byte s[8])
{
//byte n[0] = s[0] & 0x01 << 7 + (s[1] & 0x01) << 6 + (s[2] & 0x01) << 5 + (s[3] & 0x01) << 4 + (s[4] & 0x01) << 3 + (s[5] & 0x01) << 2 + (s[6] & 0x01) << 1 + (s[7] & 0x01);
//byte n[1] = s[0] & 0x02 << 6 + (s[1] & 0x02) << 5 + (s[2] & 0x02) << 4 + (s[3] & 0x02) << 3 + (s[4] & 0x02) << 2 + (s[5] & 0x02) << 1 + (s[6] & 0x02)      + (s[7] & 0x02) >> 1;
//byte n[2] = s[0] & 0x04 << 5 + (s[1] & 0x04) << 4 + (s[2] & 0x04) << 3 + (s[3] & 0x04) << 2 + (s[4] & 0x04) << 1 + (s[5] & 0x04)      + (s[6] & 0x04) >> 1 + (s[7] & 0x04) >> 2;

  byte n[8];
  for (byte i=0; i<8; i++)
  {
    byte mask = 1 << i;
    n[i] = (s[0] & mask) << (7-i) +
           (s[1] & mask) << (6-i) +
           (s[2] & mask) << (5-i) +
           (s[3] & mask) << (4-i) +
           (s[4] & mask) << (3-i) +
           (s[5] & mask) << (2-i) +
           (s[6] & mask) << (1-i) +
           (s[7] & mask) <<   -i;
  }
}


void rotate180(byte s[8])
{
//byte n[0] = s[7] & 0x01 << 7 + (s[7] & 0x02) << 6 + (s[7] & 0x04) << 5 + (s[7] & 0x08) << 4 + (s[7] & 0x10) << 3 + (s[7] & 0x20) << 2 + (s[7] & 0x40) << 1 + (s[7] & 0x80);
//byte n[1] = s[6] & 0x01 << 6 + (s[6] & 0x02) << 5 + (s[6] & 0x04) << 4 + (s[6] & 0x08) << 3 + (s[6] & 0x10) << 2 + (s[6] & 0x20) << 1 + (s[6] & 0x40)      + (s[6] & 0x80) >> 1;
//byte n[2] = s[5] & 0x01 << 5 + (s[5] & 0x02) << 4 + (s[5] & 0x04) << 3 + (s[5] & 0x08) << 2 + (s[5] & 0x10) << 1 + (s[5] & 0x20)      + (s[5] & 0x40) >> 1 + (s[5] & 0x80) >> 2;
//byte n[3] = s[4] & 0x01 << 4 + (s[4] & 0x02) << 3 + (s[4] & 0x04) << 2 + (s[4] & 0x08) << 1 + (s[4] & 0x10)      + (s[4] & 0x20) >> 1 + (s[4] & 0x40) >> 2 + (s[4] & 0x80) >> 3;

  byte n[8];
  for (byte i=0; i<8; i++)
  {
    byte t = 7-i;
    n[i] = (s[7-i] & 0x01) << (7-i) +
           (s[7-i] & 0x02) << (6-i) +
           (s[7-i] & 0x04) << (5-i) +
           (s[7-i] & 0x08) << (4-i) +
           (s[7-i] & 0x10) << (3-i) +
           (s[7-i] & 0x20) << (2-i) +
           (s[7-i] & 0x40) << (1-i) +
           (s[7-i] & 0x80) << (-i);
  }
}



byte readByte(int i2cId, unsigned int eeaddress)
{
  byte rdata = 0xFF;
  Wire.beginTransmission(i2cId);
  Wire.send((int)(eeaddress >> 8));    // Address High Byte
  Wire.send((int)(eeaddress & 0xFF));  // Address Low Byte
  Wire.endTransmission();
  Wire.requestFrom(i2cId, 1);
  if (Wire.available()) rdata = Wire.receive();
  return rdata;
}


byte readNextByte()
{
  byte rdata = readByte(I2C_ID, crtReadAddress);
  crtReadAddress++;
  return rdata;
}


// read, from I2C EEPROM, a whole line (ending with CR) into the message buffer;    
void fetchLineFromEprom()
{
  byte lastReadByte = readNextByte();

  // after reaching the end of eprom's content, start from the beginning;
  if (lastReadByte == 0xFF || lastReadByte == 0)
  {
    crtReadAddress = 0;
    lastReadByte   = readNextByte();
  }

  while (lastReadByte != 13 && lastReadByte != 0xFF && lastReadByte != 0)
  {
    *msgLinePtr++ = lastReadByte;
    lastReadByte  = readNextByte();
  }

  // insert a few blanks (pause between messages);
  *msgLinePtr++ = ' ';
  *msgLinePtr++ = ' ';
  *msgLinePtr++ = ' ';

  // mark the end of the string;
  *msgLinePtr++ = 0;

//  Serial.println(msgLine);
  msgLinePtr  = &msgLine[0];      // reset the string pointer;
  msgLineSize = strlen(msgLine);  // update the size of the current string;
}


//------------------------------------------------------------------------
// put the arduino to sleep to save power;
// function copied from http://www.arduino.cc/playground/Learning/arduinoSleepCode;
//------------------------------------------------------------------------
void sleepNow()
{
    /* Now is the time to set the sleep mode. In the Atmega8 datasheet
     * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
     * there is a list of sleep modes which explains which clocks and 
     * wake up sources are available in which sleep modus.
     *
     * In the avr/sleep.h file, the call names of these sleep modus are to be found:
     *
     * The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings 
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN     -the most power savings
     *
     * For now, we want as much power savings as possible, so we 
     * choose the according 
     * sleep modus: SLEEP_MODE_PWR_DOWN
     * 
     */  
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here

    sleep_enable();          // enables the sleep bit in the mcucr register so sleep is possible; just a safety pin;

    detachInterrupt(0);      // disables the previous routine (IR receiver ISR);
    attachInterrupt(0, wakeUpNow, LOW); // use interrupt 0 (pin 2) and run function wakeUpNow when pin 2 gets LOW (IR command);

#ifdef _DEBUG_
    Serial.println("going to sleep...");
#endif

    sleep_mode();        // the device is actually put to sleep! THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP.

#ifdef _DEBUG_
    Serial.println("I am back!");
#endif

    sleep_disable();         // first thing after waking from sleep: disable sleep...

    detachInterrupt(0);      // disables interrupt 0 on pin 2 so the wakeUpNow code will not be executed during normal running time;
    attachInterrupt(0, irReceiverISR, LOW);
}


void wakeUpNow()        // here the interrupt is handled after wakeup
{
  // execute code here after wake-up before returning to the loop() function
  // timers and code using timers (serial.print and more...) will not work here.
  // we don't really need to execute any special functions here, since we just want the thing to wake up;
}


// function called when the 'Q' menu item is selected;
void displayingQuote()
{
        page = RED;

#ifdef _16x8_
        displayAndScroll_16x8(msgLine[msgLineIndex]);
#else
        displayAndScroll(msgLine[msgLineIndex]);
#endif

        msgLineIndex++;
        if (msgLineIndex >= msgLineSize)
        {
          fetchLineFromEprom();
          msgLineIndex = 0;
        }
}


// function called when the 'T' menu item is selected;
void displayingTime()
{
        page = ORANGE;
        updateTimeBuffer();

#ifdef _16x8_
        displayAndScroll_16x8(timeBuffer[timeBufferPtr]);
#else
        displayAndScroll(timeBuffer[timeBufferPtr]);
#endif

        timeBufferPtr++;
        if (timeBufferPtr >= timeBufferSize)
        {
          timeBufferPtr = 0;

          // updateTemperature should be called less often, e.g. every 10 seconds;
          // use a global counter to keep track;
          nCounter++;
          nCounter = nCounter % 5;
          //if (nCounter == 0)  updateTemperature();
        }
}


// function called when the 'B' menu item is selected;
void displayingBothQuoteAndTime()
{
  if (isDisplayingTime)
  {
    displayingTime();

    // when time buffer pointer wraps up, switch display to quote;
    if (timeBufferPtr == 0)
    {
      isDisplayingTime  = false;
      isDisplayingQuote = true;
      delay(800);
    }
  }

  if (isDisplayingQuote)
  {
    displayingQuote();

    // when msg buffer index wraps up, switch display to time;
    if (msgLineIndex == 0)
    {
      isDisplayingQuote = false;
      isDisplayingTime  = true;
      delay(800);
    }
  }
}


// function called when the 'A' menu item is selected;
void setAlarm()
{
  int hour, minute;
  int onOff;	// either 1 (on) or 0 (off);

  byte bellOn[8] =
  {
    0x00,    // ________
    0x18,    // ___XX___
    0x24,    // __X__X__
    0x24,    // __X__X__
    0x42,    // _X____X_
    0xFF,    // XXXXXXXX
    0x08,    // ____X___
    0x18,    // ___XX___
  };

  byte bellOff[8] =
  {
    0x00,    // ________
    0x18,    // ___XX___
    0x24,    // __X__X__
    0x24,    // __X__X__
    0x42,    // _X____X_
    0xFF,    // XXXXXXXX
    0x00,    // ________
    0x00,    // ________
  };


  // REM: display alarm ON or OFF (bell);
  // also display the alarm time (in red);


  if (Serial.available() > 0)
  {
    int onOff = Serial.read();
    if (onOff == 1)
    {
      // read the alarm time if the alarm is set to ON;
      int hour   = Serial.read();
      int minute = Serial.read();
    }
  }

  // write settings to internal EEPROM (or RTC ram?);
  storeAlarmSettings(onOff, hour, minute);
}


void storeAlarmSettings(int onOff, int hour, int minute)
{
  // REM: to implement;
}


// function called when the 'L' (Load) menu item is selected;
void setMessage()
{
  // REM: display rainingDots to show processing (reading the message from serial port);

  // REM: need to know the address where to write the incoming bytes;
  // may be set by the sending app (could be also 0, if we want to overwrite the existing content);
  if (Serial.available() > 0)
  {
      int incomingByte = Serial.read();
      writeNextByte(incomingByte);

#ifdef _DEBUG_
      Serial.print("wrote to addr ");
      Serial.print(crtWriteAddress);
      Serial.print("  byte ");
      Serial.println(incomingByte, BYTE);
#endif
  }
}


// (fc, Feb 18/09) create the effect of raining green dots, like in the movie "matrix";
void rainingDots()
{
  page = GREEN;  // set the current page (color);

  randomSeed(millis());

  while (true)
  {
    // shift down the top 7 rows;
    for (byte i = 0; i < 7; i++)
    {
      screenMem[i+8 + 1] = screenMem[i+8];
    }

    // generate a random pattern for the top row (0..255);
    screenMem[0+8] = random(0, 256);

    delay(50);
  }
}


// used for writing to the external EEPROM (on I2C);
void writeNextByte(byte data)
{
  int rdata = data;
  Wire.beginTransmission(I2C_ID);
  Wire.send((int)(crtWriteAddress >> 8));    // Address High Byte
  Wire.send((int)(crtWriteAddress & 0xFF));  // Address Low Byte
  Wire.send(rdata);
  Wire.endTransmission();
  delay(10);        // delay is required according to specs;

  crtWriteAddress++;
}


void setTime()
{
  Serial.print("set time to ");
  Serial.print(timeDigit[0]);
  Serial.print(timeDigit[1]);
  Serial.print(":");
  Serial.print(timeDigit[2]);
  Serial.print(timeDigit[3]);
  Serial.println("");
  
  
  RTC.stop();
  RTC.set(DS1307_SEC, 0);
  RTC.set(DS1307_MIN, timeDigit[2] * 10 + timeDigit[3]);
  RTC.set(DS1307_HR,  timeDigit[0] * 10 + timeDigit[1]);
  // dummy values, since we don't display them anyway;
  RTC.set(DS1307_DOW,  1);
  RTC.set(DS1307_DATE, 1);
  RTC.set(DS1307_MTH,  1);
  RTC.set(DS1307_YR,   9);
  RTC.start();
}


void bigOptionSwitch()
{
    switch (menuOption)
    {
      case OPTION_QUOTE:
        displayingQuote();
        break;

      case OPTION_TIME:
        displayingTime();
        break;

      case OPTION_BOTH_Q_T:
        // alternate display between quote and time;
        displayingBothQuoteAndTime();
        break;

      case OPTION_SLEEP:
        // used for power saving; it puts the processor in "sleep mode power down";
        resetDisplay();
        delay(100);     // this delay is needed;
        sleepNow();
        break;

      // used for setting up the RTC's time;
      case OPTION_SETTIME:
        // expects 4 digits (HHMM) from the remote;
        // once they are entered, the mode changes to OPTION_TIME;
        // numeric keys are handled in the ISR (indexTimeDigit gets incremented there);
        if (indexTimeDigit > 3)
        {
          setTime();                 // set the time in RTC;
          menuOption = OPTION_TIME;  // quit setting time;
          indexTimeDigit = 0;
        }
        break;

#ifdef OPTION_SPEED
      case OPTION_SPEED:
        // choose a number on a scale from 1 to 8, displayed as ascending histogram;
        // this value is then used to set the scrolling speed;
        if (eventTime == 0)   eventTime = millis();
        
        showAdjustingLevel();
        delay(1000);

        // we should get out of this menu option automatically after a time interval (3 seconds);
        if (millis() - eventTime > 3000)
        {
          eventTime = 0;
          menuOption = DEFAULT_OPTION;
          resetDisplay();
        }
        break;
#endif

#ifdef OPTION_CONWAY
      case OPTION_CONWAY:
        if (isNewGame)
        {
          ConwayGame.initGeneration();
          isNewGame = false;
        }

        setScreenMem(GREEN, ConwayGame.getCurrentGeneration());
        ConwayGame.calculateNewGeneration();
        delay(1000);
        break;
#endif

#ifdef OPTION_DICE
      case OPTION_DICE:
        showDice();
        break;
#endif

#ifdef OPTION_TENGU
      case OPTION_TENGU:
        showTengu();
        delay(400);
        break;
#endif

#ifdef OPTION_ALARM
      case OPTION_ALARM:
        // used for setting up the RTC (time and alarm) from a windows app sending data on serial port;
        setAlarm();
        break;
#endif

#ifdef OPTION_LOAD
      case OPTION_LOAD:
        // used for loading messages (quotations) from a windows app sending data on serial port;
        setMessage();
        break;
#endif

      default:
        // nothing for now;
        break;
    }
}



