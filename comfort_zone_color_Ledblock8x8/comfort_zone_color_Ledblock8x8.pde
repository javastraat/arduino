/************************************

an arduino application for the RGB LED matrix 8x8 with shield from ITEAD Studio

needed
arduino
shield
RGB LED Block

distance sensor
SRF02

soldering 
only the SF02 wiring

DIY: change the patterns of course!

This sketch shows different patterns or smileys for different distances.
It shifts the patterns slowly, it takes time to find the heart!

it can be used (for men only!) to indicate the distance other people have to keep from them to feel comfortable for the wearer

(in discussions with women about this idea, they told us that they know this distance intuitively
and are offended to leave this "distance sensing" to electronic devices!!!)

you can change the patterns, also for the changes within the patterns there are different examples, like shifting or alternating

www.contrechoc.com
2011

This library allows you to communicate with I2C / TWI devices. On most Arduino boards, SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5. On the Arduino Mega, SDA is digital pin 20 and SCL is 21. 

connections:
SDA of SRF02 to analog 5 SCL (clock line)
SCL of SRF02 to analog 4 SDA (data line)
SRF02 mode connection: connect to SRF02 VCC

link about colorduino:
link about SRF02:

this sketch is made available at www.instructables.com

*************************************/

#include <Colorduino.h>

const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;
int buttonState = 0;

int iconMode = 0;
unsigned char iconMode0 = 0;

long timer;
long interval = 10;

unsigned char smileyNumber;
unsigned char rainC = 0;

int oldDistance = 0;

unsigned char rainArray[8] =  { \
B01000100,\
B01000100,\
B00010001,\
B01010001,\
B00010001,\
B00000100,\
B01000100,\
B01000000,\
}; 

unsigned char smileyArray0[8] =  { \
B01000010,\
B11100111,\
B10101001,\
B00000000,\
B10000001,\
B01100110,\
B00111100,\
B00011000,\
}; 

unsigned char smileyArray1[8] =  { \
B11100111,\
B10100101,\
B11000011,\
B00011000,\
B11000011,\
B11111111,\
B01100110,\
B00111100,\
}; 

unsigned char sun0[8] =  { \
B10010001,\
B01010010,\
B00111100,\
B11111100,\
B00111111,\
B00111100,\
B01001010,\
B10001001,\
}; 

unsigned char sun1[8] =  { \
B01001000,\
B00101001,\
B00011010,\
B11111100,\
B00111111,\
B01011000,\
B10010100,\
B00010010,\
};

unsigned char heartArray1[8] =  { \
B01100110,\
B11111111,\
B11111111,\
B11111111,\
B11111111,\
B01111110,\
B00111100,\
B00011000,\
};  

unsigned char heartArray2[8] =  { \
B00000000,\
B00000000,\
B00000000,\
B00011000,\
B00111100,\
B00011000,\
B0000000,\
B00000000,\
}; 

unsigned char noiseArray0[8] =  { \
B10101010,\
B01010101,\
B10101010,\
B01010101,\
B10101010,\
B01010101,\
B10101010,\
B01010101,\
};  
 
 unsigned char noiseArray1[8] =  { \
B01010101,\
B10101010,\
B01010101,\
B10101010,\
B01010101,\
B10101010,\
B01010101,\
B10101010,\
};  
 

typedef struct
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
} ColorRGB;

//a color with 3 components: h, s and v
typedef struct
{
  unsigned char h;
  unsigned char s;
  unsigned char v;
} ColorHSV;

unsigned char plasma[ColorduinoScreenWidth][ColorduinoScreenHeight];
long paletteShift;
int inByte = 0;
unsigned char speedOfChange = 1;

//declarations for distance sensor
#include <Wire.h>
#define sensorAddress 0x70
#define readInches 0x50
#define readCentimeters 0x51
#define readMicroseconds 0x52
#define resultRegister 0x02
#define minimumRegister 0x04

void setup()
{
  
   Serial.begin(9600);   //for debudding
   initializeColorduino();
   Wire.begin();
 
}

int readDistance(){
    // send the command to read the result in inches:
  sendCommand(sensorAddress, readCentimeters);
  // wait at least 70 milliseconds for a result:
  delay(70);
  // set the register that you want to read the result from:
  setRegister(sensorAddress, resultRegister);
// read the result:
  int sensorReading = readData(sensorAddress, 2);
  if ( sensorReading > (oldDistance + 1) ) oldDistance += speedOfChange;// speedOfChange, can be made bigger than 1 to make the changes go faster
  if ( sensorReading < (oldDistance - 1) ) oldDistance -= speedOfChange;// speedOfChange, can be made bigger to make the changes go faster
  
  //returning the iconMode number that is the animation which will be shown depending on the distance
  return oldDistance/28;
}

void loop()
{
   //depending on iconMode you get an animation
    iconMode = readDistance();
  delay(100); //this delay makes the changes go faster or slower
   
  if ( iconMode == 0)//to close: noise!
  {
      if ( iconMode0 == 0 ){
        iconMode0 = 1;
        setNoise0(255*random(),255*random(),255*random());
      }else{
         iconMode0 = 0;
         setNoise1(255*random(),255*random(),255*random());
      }
  }

  if ( iconMode == 1)//heart
  {
    setHeart();
  }
  
  if ( iconMode == 2)//smiley
  {
       smiley(255*random(),255*random(),255*random(), smileyNumber); 
       if ( timer < millis() )
        {
          timer = millis() + interval + 400;
          if ( smileyNumber == 0 )
          smileyNumber = 1 ;
          else
          smileyNumber = 0 ;
        }
  }
  
  if ( iconMode == 3)//rain
  {
    rain(255*random(),255*random(),255*random());
    if ( timer < millis() )
        {
       
          timer = millis() + interval + random(100);
          /*
          unsigned temp = rainArray[0];
          for(int y = 0; y < ColorduinoScreenHeight - 1; y++)  rainArray[y] = rainArray[y + 1];
          rainArray[7] = temp;
          */
          unsigned temp = rainArray[7];
          for(int y = ColorduinoScreenHeight - 1; y > 0; y--)  rainArray[y] = rainArray[y - 1];
          rainArray[0] = temp;
          
          rainC++;
          if ( (rainC%8) == 0)
          for(int y = 0; y < ColorduinoScreenHeight; y++) rainArray[y] = shiftBIN (  rainArray[y]  );
        }
  }

  if ( iconMode == 5)  ColorFill(255,0,0); // colors which fade in and out of the existing pattern
  
   if ( iconMode == 6) {
               sun(255*random(),255*random(),255*random(), smileyNumber); 
               if ( timer < millis() )
                {
                        timer = millis() + interval + 400;
                        if ( smileyNumber == 0 )
                        smileyNumber = 1 ;
                        else
                        smileyNumber = 0 ;
                }
       }

   if ( iconMode == 9)  ColorFill(255,255,0); // colors which fade in and out of the existing pattern
   if ( iconMode == 4)  ColorFill(255,0,255);
   if ( iconMode == 7)  ColorFill(0,255,255);
   if ( iconMode == 8)  ColorFill(0,255,0);

  if (Serial.available() > 0) {   //testing
 
    inByte = Serial.read();  

    switch(inByte) {
      case '0': iconMode = 0; break;
      case '1': iconMode = 1; break;  
      case '2': iconMode = 2; break;  
      case '3': iconMode = 3; break;   
      case '4': iconMode = 4; break;   
      case '5': iconMode = 5; break; 
      case '6': iconMode = 6; break;   
      case '7': iconMode = 7; break;   
      case '8': iconMode = 8; break;   
      case '9': iconMode = 9; break;   
    }
  }
    Serial.flush();
}



