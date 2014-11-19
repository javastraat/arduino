#ifndef _COLORDUINO_GFX_H_
#define _COLORDUINO_GFX_H_

#define COLORDUINO_GFX_MAJOR 1
#define COLORDUINO_GFX_MINOR 0
#define COLORDUINO_GFX_PATCH 0

#define COLORDUINO_GFX_VERSION (COLORDUINO_GFX_MAJOR * 10000 \
		              + COLORDUINO_GFX_MINOR * 100 \
		              + COLORDUINO_GFX_PATCH)

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
 #include "pins_arduino.h"
#endif

#include <avr/pgmspace.h> 
#include <avr/io.h>
#include <avr/interrupt.h>

#define ColorduinoBitsPerColor 8

#define ColorduinoScreenWidth 8
#define ColorduinoScreenHeight 8

/*****************************
define the IO
*****************************/
#if defined(ARDUINO) && ARDUINO >= 100
#define RST_BIT digitalPinToBitMask(A2)
#define LAT_BIT digitalPinToBitMask(A1)
#define SLB_BIT digitalPinToBitMask(A0)
#define SCL_BIT digitalPinToBitMask(6)
#define SDA_BIT digitalPinToBitMask(7)
#define RST* portOutputRegister(digitalPinToPort(A0))
#define LAT* portOutputRegister(digitalPinToPort(A1))
#define SLB* portOutputRegister(digitalPinToPort(A2))
#define SCL* portOutputRegister(digitalPinToPort(6))
#define SDA* portOutputRegister(digitalPinToPort(7))
#define open_line0	{*portOutputRegister(digitalPinToPort(8))=digitalPinToBitMask(8);}
#define open_line1	{*portOutputRegister(digitalPinToPort(9))=digitalPinToBitMask(9);}
#define open_line2	{*portOutputRegister(digitalPinToPort(10))=digitalPinToBitMask(10);}
#define open_line3	{*portOutputRegister(digitalPinToPort(11))=digitalPinToBitMask(11);}
#define open_line4	{*portOutputRegister(digitalPinToPort(12))=digitalPinToBitMask(12);}
#define open_line5	{*portOutputRegister(digitalPinToPort(13))=digitalPinToBitMask(13);}
#define open_line6	{*portOutputRegister(digitalPinToPort(3))=digitalPinToBitMask(3);}
#define open_line7	{*portOutputRegister(digitalPinToPort(4))=digitalPinToBitMask(4);}

#if defined (__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define close_all_lines {PORTB&=0b00001111;PORTG&=0b11011111;PORTE&=0b11011111;PORTH&=0b10011111;}
#elif defined(__AVR_ATmega32U4__)
#define close_all_lines {PORTB&=0b00001111;PORTC&=0b01111111;PORTD&=0b10101110;}
#else
#define close_all_lines	{PORTB&=0b11000000;PORTD&=0b11000000;}
#endif
#else // !arduino 1.0
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#define RST PORTF // A2 = PF2
#define RST_BIT 0x04
#define LAT PORTF // A1 = PF1
#define LAT_BIT 0x02
#define SLB PORTF // A0 = PF0
#define SLB_BIT 0x01
#define SCL PORTH // D6 = PH3
#define SCL_BIT 0x08
#define SDA PORTH // D7 = PH4
#define SDA_BIT 0x10

#define open_line0	{PORTH=0x20;} // D8 = PH5
#define open_line1	{PORTH=0x40;} // D9 = PH6
#define open_line2	{PORTB=0x10;} // D10 = PB4
#define open_line3	{PORTB=0x20;} // D11 = PB5
#define open_line4	{PORTB=0x40;} // D12 = PB6
#define open_line5	{PORTB=0x80;} // D13 = PB7
#define open_line6	{PORTE=0x20;} // D3 = PE5
#define open_line7	{PORTG=0x20;} // D4 = PG5
#define close_all_lines	{PORTH=0x00;PORTB=0x00;PORTE=0x00;PORTG=0x00;}
#elif defined(__AVR_ATmega32U4__)
#else // 328/168

#define RST PORTC // A2 = PC2
#define RST_BIT 0x04
#define LAT PORTC // A1 = PC1
#define LAT_BIT 0x02
#define SLB PORTC // A0 = PC0
#define SLB_BIT 0x01
#define SCL PORTD // D6 = PD6
#define SCL_BIT 0x40
#define SDA PORTD // D7 = PD7
#define SDA_BIT 0x80

#define open_line0	{PORTB=0x01;} // D8 = PB0
#define open_line1	{PORTB=0x02;} // D9 = PB1
#define open_line2	{PORTB=0x04;} // D10 = PB2
#define open_line3	{PORTB=0x08;} // D11 = PB3
#define open_line4	{PORTB=0x10;} // D12 = PB4
#define open_line5	{PORTB=0x20;} // D13 = PB5
#define open_line6	{PORTD=0x08;} // D3 = PD3
#define open_line7	{PORTD=0x10;} // D4 = PD4
#define close_all_lines	{PORTD=0x00;PORTB=0x00;}

#endif  // defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#endif // defined(ARDUINO) && ARDUINO >= 100

#define LED_RST_SET RST|=RST_BIT
#define LED_RST_CLR RST&=~RST_BIT
#define LED_SDA_SET SDA|=SDA_BIT
#define LED_SDA_CLR SDA&=~SDA_BIT
#define LED_SCL_SET SCL|=SCL_BIT
#define LED_SCL_CLR SCL&=~SCL_BIT
#define LED_LAT_SET LAT|=LAT_BIT
#define LED_LAT_CLR LAT&=~LAT_BIT
#define LED_SLB_SET SLB|=SLB_BIT
#define LED_SLB_CLR SLB&=~SLB_BIT

#include "Adafruit_GFX.h"

#ifndef GFX_COLOR_MODE
#define GFX_COLOR_16BIT 0
#define GFX_COLOR_8BIT 1
#define GFX_COLOR_RGB24BIT 2
#define GFX_COLOR_MODE 0
typedef uint16_t GFX_Color_t;
typedef int16_t GFX_Coord_t;
#endif

// this type is for internal use only
typedef struct {uint8_t r; uint8_t g; uint8_t b;} Colorduino_Color_t;

class ColorduinoPanel : public Adafruit_GFX {
  public:
    Colorduino_Color_t *writeBuffer;
    Colorduino_Color_t *activeBuffer;

    uint8_t line;

    ColorduinoPanel();
    ColorduinoPanel(int height, int width);

    void
      begin(void),
      drawPixel(GFX_Coord_t x, GFX_Coord_t y, GFX_Color_t color),
      init(void),
      init_io(void),
      init_led(void),
      init_tc2(void),
      LED_Delay(unsigned char i),
      fillColor(GFX_Color_t color),
      openLine(unsigned char x),
      run(void),
      setWhiteBalance(uint8_t r, uint8_t g, uint8_t b),
      stop(void),
      swapBuffers(boolean copy);

    GFX_Color_t
      color(uint8_t r, uint8_t g, uint8_t b);
    GFX_Color_t
      // get a pixel for writing in the offscreen framebuffer
      getActivePixel(GFX_Coord_t x, GFX_Coord_t y),
      // get a pixel from the active framebuffer
      getWritePixel(GFX_Coord_t x, GFX_Coord_t y);

  private:
    Colorduino_Color_t
      *_getActivePixel(GFX_Coord_t x, GFX_Coord_t y),
      *_getWritePixel(GFX_Coord_t x, GFX_Coord_t y);

};
#endif
