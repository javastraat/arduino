/*
* This sketch is to be used on either one of the 2 kinds of 24LC256 shields:
*  1. one standalone, with read/write button on (input) pin 10 and read/write LED om (output) pin 12;
*  2. one used in the WiseClock project, with no button and LED;
*
* For case #1, set WISE_CLOCK to false; for case #2, set WISE_CLOCK to true.
* When in the WiseClock, the default (starting) mode is "write", that is,
* the incoming bytes on COM port will be written to the eprom.
*
* This sketch works in conjunction with "writeFileToCom.vbs" script (also included at the bottom of this file),
* which reads the content of a text file and sends it to the specified COM.
* For the script to work, the serial monitor in Arduino IDE MUST be off, so the COM port can be used by the script alone.
*
* The mode is changed to "read" when the characters "\r" or "\l" are encountered.
* "\r" will display the eprom content as individual characters (one character per line).
* "\l" will display the eprom content line by line, each line ending with CR (13).
*/

#include <Wire.h>


/* Simple read & write to a 24LC256 EEPROM using the Wire library.
 * Addresses are ints - 0000-7FFF (32767) Data is bytes (8 bits x 32767 = 256K)
 * Functions for R/W of single byte or a page of bytes. Max page is 28 bytes.
 *
 * SETUP:           _ _                                
 * Arduino GND- A0-|oU |-Vcc  to Arduino Vcc
 * Arduino GND- A1-|   |-WP   to GND for now. Set to Vcc for write protection.
 * Arduino GND- A2-|   |-SCL  to Arduino 5
 * Arduino GND-Vss-|   |-SDA  to Arduino 4
 *                  ---       (A2, A1, A0 to GND for 1010000 (0x50) address.)
 *                            If set to Vcc adds to address (1010,A2,A1,A0)
 */


// set to true when using the shield which is part of the Life clock sandwich;
// set to false when using a shield with read/write button on pin 10 and read/write LED on pin 12;
#define WISE_CLOCK  true


#define BTN_WRITE_IN_PIN   10
#define LED_WRITE_OUT_PIN  12


// status gets changed by pressing the button (on pin 10) or by sending the command "\r" or "\l";
boolean isWrite;


// I2C Bus address of 24LC256 EEPROM;
// if more than one eeprom, they will have different addresses (h/w configured);
#define I2C_ID 0x50


// global address of the last written byte;
unsigned int crtWriteAddress = 0;

// global address of the last read byte;
unsigned int crtReadAddress = 0;


void setup()
{
  if (!WISE_CLOCK)
  {
    pinMode(BTN_WRITE_IN_PIN,  INPUT);
    pinMode(LED_WRITE_OUT_PIN, OUTPUT);
  
    // initial state is READ;
    isWrite = false;
    digitalWrite(LED_WRITE_OUT_PIN, LOW);
  }
  else
  {
    isWrite = true;
  }

  Wire.begin();                        // join I2C bus (address optional for master)
  Serial.begin(9600);
  Serial.println(isWrite ? "write eprom" : "read eprom");

  crtReadAddress  = 0;
  crtWriteAddress = 0;
}


void writeByte(int i2cId, unsigned int eeaddress, byte data)
{
  int rdata = data;
  Wire.beginTransmission(i2cId);
  Wire.send((int)(eeaddress >> 8));    // Address High Byte
  Wire.send((int)(eeaddress & 0xFF));  // Address Low Byte
  Wire.send(rdata);
  Wire.endTransmission();
  delay(10);                         // NEED THIS DELAY!
}


void writeNextByte(byte data)
{
  writeByte(I2C_ID, crtWriteAddress, data);
  crtWriteAddress++;
}


// Address is a page address, 6-bit (0..63). More and end will wrap around.
// But data can be maximum of 28 bytes, because the Wire library has a buffer of 32 bytes.
void writePage(int i2cId, unsigned int eeaddresspage, byte* data, byte length)
{
  Wire.beginTransmission(i2cId);
  Wire.send((int)(eeaddresspage >> 8));   // Address High Byte
  Wire.send((int)(eeaddresspage & 0xFF)); // Address Low Byte

  for (byte i = 0; i < length; i++)
    Wire.send(data[i]);

  Wire.endTransmission();
  delay(10);                           // need some delay
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


// should not read more than 28 bytes at a time!
void readPage(int i2cId, unsigned int eeaddress, byte *buffer, int length)
{
  Wire.beginTransmission(i2cId);
  Wire.send((int)(eeaddress >> 8));    // Address High Byte
  Wire.send((int)(eeaddress & 0xFF));  // Address Low Byte
  Wire.endTransmission();
  Wire.requestFrom(i2cId, length);

  for (int i = 0; i < length; i++ )
    if (Wire.available()) buffer[i] = Wire.receive();
}


//-------------------------------------------------------------------------------------
// used for the standalone shield, with read/write button (pin 10) and LED (pin 12);

#define BOUNCE_TIME_BUTTON  200   // bounce time in ms for the WRITE button;
volatile unsigned int btnPressTime = 0;

// executed as a result of the menu button being pressed;
// determines the menu to be displayed and the menu option to be increased;
void processWriteButton()
{
  // debouncing;
  if (abs(millis() - btnPressTime) < BOUNCE_TIME_BUTTON)
    return;

  btnPressTime = millis();

  isWrite = !isWrite;
  digitalWrite(LED_WRITE_OUT_PIN, (isWrite? HIGH : LOW));

  crtReadAddress  = 0;
  crtWriteAddress = 0;
}
//-------------------------------------------------------------------------------------


byte lastReadByte = 0;

// current line string is built by copying each character to the position of the pointer, then advancimng the pointer;
char msgBuffer[200] = {0};
char* msgBufferPtr = &msgBuffer[0];

boolean isCommand = false;
boolean isReadLineByLine = false;


void loop()
{
  if (!WISE_CLOCK)
  {
    // read the state of the WRITE button;
    if (LOW == digitalRead(BTN_WRITE_IN_PIN))
        processWriteButton();
  }

  if (isWrite)
  {
    if (Serial.available() > 0)
    {
      // read next character from COM port;
      byte incomingByte = Serial.read();

      if (isCommand && (incomingByte == 'r' || incomingByte == 'l'))
      {
        isWrite = false;
        Serial.println("read eprom");

        switch (incomingByte)
        {
          case 'r':
            isReadLineByLine = false;
            break;
            
          case 'l':
            isReadLineByLine = true;
            break;
        }
      }
      else
      {
        isCommand = false;

        if (incomingByte == '\\')
        {
          // don't write the backslash;
          isCommand = true;
        }
        else
        {
// the following debug lines (serial.print...) take about 25 more ms than the write itself (10 ms);
// it is a good idea to comment them out if we transfer the content (32K) of a whole file;
/*
          Serial.print("write to addr ");
          Serial.print(crtWriteAddress);
          Serial.print("  byte ");
          Serial.println(incomingByte, BYTE);
*/
          writeNextByte(incomingByte);
        }
      }
    }
  }
  else  // reading the eeprom;
  {
    if (isReadLineByLine)
    {
      // read a whole line into the message buffer;    
      lastReadByte = readNextByte();
      while (lastReadByte != 13 && lastReadByte != 0xFF)
      {
        *msgBufferPtr++ = lastReadByte;
        lastReadByte = readNextByte();
      }
      *msgBufferPtr++ = 0;

      Serial.println(msgBuffer);
      msgBufferPtr = &msgBuffer[0];  // reset buffer pointer;
      delay(50);
    }
    else
    {
      if (0xFF != lastReadByte)
      {
        Serial.print("read from addr ");
        Serial.print(crtReadAddress);
        Serial.print("  byte ");
        lastReadByte = readNextByte();
        Serial.print(lastReadByte, BYTE);
        Serial.print(" (");
        Serial.print(lastReadByte, DEC);
        Serial.println(")");
        delay(50);
      }
    }
  }
}



/****************************************************************************************
Const ForReading = 1
Const ForWriting = 2

'------------------------------------------------------------------------------------------------
' open USB serial port (COMx);
'
' If the serial monitor in Arduino IDE is open, you will get an "access denied" error.
' Just make sure that the serial monitor is closed (so bytes are not sent by the arduino board).
'------------------------------------------------------------------------------------------------

Set fso = CreateObject("Scripting.FileSystemObject")
Set com = fso.OpenTextFile("COM3:9600,N,8,1", ForWriting)


'---------------------------------------------
' read content of text file line by line;
' write line to COMx;
'---------------------------------------------

Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objFile = objFSO.OpenTextFile("C:\docs\quotes.txt", ForReading)

MsgBox("Ready to write file content to COM")

   Do While objFile.AtEndOfStream <> True
      '---------------------------------------------------------------------------------------------------------
      ' read 10 characters at a time; arduino serial buffer cannot take more than 32 characters;
      ' writing a character to eeprom takes about 11 ms (assuming that there is no serial.prints in the loop);
      ' therefore, after each batch of 10 chars sent to COM, we should wait no less than 110 ms;
      ' we use 200 to have a margin of safety;
      '---------------------------------------------------------------------------------------------------------
      strChars = objFile.Read(10)
      com.Write(strChars)
      WScript.Sleep(200)
   Loop


objFile.Close
com.Close()

MsgBox("Finished writing to COM")

*********************************************************************************/