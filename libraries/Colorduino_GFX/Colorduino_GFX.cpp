/*
  Colorduino_GFX - Colorduino_GFX Library for Arduino
  Copyright (c) 2013 Philipp Seidel <phibo@dinotools.org>
  based on C code by zzy@iteadstudio and Sam C. Lin
    Copyright (c) 2010 zzy@IteadStudio.  All right reserved.
    Copyright (c) 2011-2012 Sam C. Lin <lincomatic@hotmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "Colorduino_GFX.h"

static ColorduinoPanel *activePanel = NULL;

ColorduinoPanel::ColorduinoPanel() : Adafruit_GFX(ColorduinoScreenWidth, ColorduinoScreenHeight) {
  line = 0;
  // create two buffers
  uint16_t len = ColorduinoScreenWidth * ColorduinoScreenHeight * sizeof(Colorduino_Color_t);
  writeBuffer = (Colorduino_Color_t *)malloc(len);
  activeBuffer = (Colorduino_Color_t *)malloc(len);
  memset(writeBuffer, 0, len);
  memset(activeBuffer, 0, len);
  activePanel = this;
}

void ColorduinoPanel::begin(void) {
  //
  activePanel = this;
}

GFX_Color_t ColorduinoPanel::color(uint8_t r, uint8_t g, uint8_t b) {
#if GFX_COLOR_MODE == GFX_COLOR_16BIT
  GFX_Color_t color;
  color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
#elif GFX_COLOR_MODE == GFX_COLOR_RGB24BIT
  GFX_Color_t color = {r, g, b};
#endif
  return color;
}

void ColorduinoPanel::drawPixel(GFX_Coord_t x, GFX_Coord_t y, GFX_Color_t color) {
  if(x > 7 || y > 7 || x < 0 || y < 0)
    return;
  
  Colorduino_Color_t *p = _getWritePixel(x,y);
#if GFX_COLOR_MODE == GFX_COLOR_16BIT
  p->r = (uint8_t)(color >> 8) & 0xF8;
  p->g = (uint8_t)(color >> 3) & 0xFC;
  p->b = (uint8_t)(color << 3);
#elif GFX_COLOR_MODE == GFX_COLOR_RGB24BIT
  p->r = color.r;
  p->g = color.g;
  p->b = color.b;
#endif
}

void ColorduinoPanel::fillColor(GFX_Color_t color) {
  Colorduino_Color_t *p = _getWritePixel(0,0);
  uint8_t r, g, b;
#if GFX_COLOR_MODE == GFX_COLOR_16BIT
  r = (uint8_t)(color >> 8) & 0xF8;
  g = (uint8_t)(color >> 3) & 0xFC;
  b = (uint8_t)(color << 3);
#elif GFX_COLOR_MODE == GFX_COLOR_RGB24BIT
  r = color.r;
  g = color.g;
  b = color.b;
#endif
  for (unsigned char y=0; y < ColorduinoScreenWidth; y++) {
    for(unsigned char x=0; x < ColorduinoScreenHeight; x++) {
      p->r = r;
      p->g = g;
      p->b = b;
      p++;
    }
  }
}

// get a pixel from the active framebuffer
GFX_Color_t ColorduinoPanel::getActivePixel(GFX_Coord_t x, GFX_Coord_t y) {
  Colorduino_Color_t *c = activeBuffer + (y * ColorduinoScreenWidth) + x;
  return color(c->r, c->g, c->b);
}

// get a pixel from the active framebuffer(private)
Colorduino_Color_t *ColorduinoPanel::_getActivePixel(GFX_Coord_t x, GFX_Coord_t y) {
  return activeBuffer + (y * ColorduinoScreenWidth) + x;
}

GFX_Color_t ColorduinoPanel::getWritePixel(GFX_Coord_t x, GFX_Coord_t y) {
  Colorduino_Color_t *c = writeBuffer + (y * ColorduinoScreenWidth) + x;
  return color(c->r, c->g, c->b);
}

Colorduino_Color_t *ColorduinoPanel::_getWritePixel(GFX_Coord_t x, GFX_Coord_t y) {
  return writeBuffer + (y * ColorduinoScreenWidth) + x;
}

void ColorduinoPanel::init(void) {
  init_io();
  init_led();
  init_tc2();
}

void ColorduinoPanel::init_io() {
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  DDRF = 0xff;
  DDRH = 0xff;
  DDRB = 0xff;
  DDRE = 0xff;
  DDRG = 0xff;

  PORTF = 0x00;
  PORTH = 0x00;  
  PORTB = 0x00;
  PORTE = 0x00;
  PORTG = 0x00;
#elif defined(__AVR_ATmega328__) || defined(__AVR_ATmega168__)
  DDRD = 0xff; // set all pins direction of PortD
  DDRC = 0xff; // set all pins direction of PortC
  DDRB = 0xff; // set all pins direction of PortB
  
  PORTD = 0x00; // set all pins output is low of PortD
  PORTC = 0x00; // set all pins output is low of PortC
  PORTB = 0x00; // set all pins output is low of PortB
#else
  uint8_t lines[] = {A0,A1,A2,3,4,6,7,8,9,10,11,12,13};

  for(int i = 0; i < 13;i++){
    pinMode(lines[i],OUTPUT);
    digitalWrite(lines[i],LOW);
  }
#endif
}

void ColorduinoPanel::init_led() {
  LED_RST_SET;
  LED_Delay(1);
  LED_RST_CLR;
  LED_Delay(1);
  LED_RST_SET;
  LED_Delay(1);

  line = 0;
}

void ColorduinoPanel::init_tc2() {
  // Arduino runs at 16 Mhz...
  // Timer Settings, for the Timer Control Register etc. , thank you internets. ATmega168 !
  // Timer2 (8bit) Settings:
  // prescaler (frequency divider) values:   CS22    CS21   CS20
  //                                           0       0      0    stopped
  //                                           0       0      1      /1 
  //                                           0       1      0      /8 
  //                                           0       1      1      /32
  //                                           1       0      0      /64 
  //                                           1       0      1      /128
  //                                           1       1      0      /256
  //                                           1       1      1      /1024
  // TCNT2 increments every  = 16MHz / prescaler

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  TCCR2B = 0x00; // Disable timer while configuring
  TCCR2A = 0x00; // Use normal mode
  TIMSK2 = 0x01; // Timer2 overflow interrupt enable
  TCNT2  = 0xff; // Reset timer to count of 255
  TCCR2B = 0x05; // Prescaler = 128
#elif defined(__AVR_ATmega32U4__)
  // set prescaler to 128 -> TCNT2 freq = 125KHz
  //TCCR4B |= (1<<CS43);
  //TCCR4B &= ~((1<<CS42)|(1<<CS41)|(1<<CS40));
  TCCR4B |= ((1<<CS42)|(1<<CS41)|(1<<CS40));
  TCCR4C &= ~(1<<PWM4D);
  //TCCR4D &= ~((1<<WGM41)|(1<<WGM40));   // Use normal mode
  TCCR4A &= ~((1<<COM4A1)|(1<<COM4A0));		// Use normal mode
  TIMSK4 |= (1<<TOIE4);      //Timer4D Overflow Interrupt Enable
  TIFR4 |= (1<<TOV4);
  OCR4C = 0xff;
  TCNT4 = 0xff;
#else
  // set prescaler to 128 -> TCNT2 freq = 125KHz
  TCCR2B |= ((1<<CS22)|(1<<CS20));
  TCCR2B &= ~((1<<CS21));   

  TCCR2A &= ~((1<<WGM21) | (1<<WGM20));   // Use normal mode
  ASSR |= (1<<AS2);       // Use internal clock - external clock not used in Arduino
  TIMSK2 |= ((1<<TOIE2) | (0<<OCIE2B));   //Timer2 Overflow Interrupt Enable
  TCNT2 = 0xff;
#endif
  sei();
}


void ColorduinoPanel::openLine(unsigned char x) {
  switch (x) {  
    case 0 : open_line0;
             break;
    case 1 : open_line1; 
             break;
    case 2 : open_line2;
             break;
    case 3 : open_line3;
             break;
    case 4 : open_line4;
             break;
    case 5 : open_line5;
             break;
    case 6 : open_line6;
             break;
    case 7 : open_line7;
             break;
    default: close_all_lines;
             break;
  }
}

void ColorduinoPanel::setWhiteBalance(uint8_t r, uint8_t g, uint8_t b) {
  LED_LAT_CLR;
  LED_SLB_CLR;
  unsigned char wbval[] = {r, g, b};
  for(unsigned char k = 0; k < ColorduinoScreenHeight; k++)
    for(unsigned char i = 3; i > 0; i--) {
      unsigned char temp = wbval[i-1]<<2;
      for(unsigned char j = 0; j < 6;j++) {
        if(temp &0x80)
          LED_SDA_SET;
        else
          LED_SDA_CLR;
        
        temp =temp << 1;
        LED_SCL_CLR;
        LED_SCL_SET;
    }
  }
  LED_SLB_SET;
}

void ColorduinoPanel::LED_Delay(unsigned char i) {
  unsigned int y;
  y = i * 10;
  while(y--);
}

void ColorduinoPanel::run(void) {
  LED_SLB_SET;
  LED_LAT_CLR;
  Colorduino_Color_t *pixel = _getActivePixel(0, 7 - line);
  for(unsigned char x = 0; x < 8; x++) {
    unsigned char temp = pixel->b;
    unsigned char p;
    for(p = 0; p < 8; p++) {
      if(temp & 0x80)
        LED_SDA_SET;
      else
        LED_SDA_CLR;
      temp <<= 1;  
      LED_SCL_CLR;
      LED_SCL_SET;
    }
    temp = pixel->g;
    for(p = 0; p < 8; p++) {
      if(temp & 0x80)
        LED_SDA_SET;
      else
        LED_SDA_CLR;
      temp <<= 1;  
      LED_SCL_CLR;
      LED_SCL_SET;
    }
    temp = pixel->r;
    for(p = 0; p < 8; p++) {
      if(temp & 0x80)
        LED_SDA_SET;
      else
        LED_SDA_CLR;
      temp <<= 1;  
      LED_SCL_CLR;
      LED_SCL_SET;
    }
    pixel++;
  }
  LED_LAT_SET;
  LED_LAT_CLR;
}

void ColorduinoPanel::swapBuffers(boolean copy) {
  cli();
  // swap frame buffers
  Colorduino_Color_t *tmp = activeBuffer;
  activeBuffer = writeBuffer;
  writeBuffer = tmp;
  if(copy == true) {
    memcpy(writeBuffer, activeBuffer, ColorduinoScreenWidth * ColorduinoScreenHeight * sizeof(Colorduino_Color_t));
  }
  sei();
}

#if defined (__AVR_ATmega32U4__)
ISR(TIMER4_OVF_vect)          //Timer4  Service 
{  
  //ISR fires every 256-TCNT4 ticks
  //so if TCNT4 = 100, ISR fires every 156 ticks
  //prescaler = 128 so ISR fires every 16MHz / 128 = 125KHz
  //125KHz / 156 = 801.282Hz / 8 rows = 100.16Hz refresh rate
  //if TCNT4 = 61, ISR fires every 256 - 61 = 195 ticks
  //125KHz / 195 = 641.026Hz / 8 rows = 80.128Hz refresh rate
  //TCNT4 = 100;
  TCNT4 = 61;
  if(activePanel == NULL) {
    return;
  }
  close_all_lines;  
  activePanel->run();
  activePanel->openLine(Colorduino.line);
  if (++activePanel->line > 7) activePanel->line = 0;
}
#else
ISR(TIMER2_OVF_vect)          //Timer2  Service 
{ 
 // ISR fires every 256-TCNT2 ticks
 // so if TCNT2 = 100, ISR fires every 156 ticks
 // prescaler = 128 so ISR fires every 16MHz / 128 = 125KHz
 // 125KHz / 156 = 801.282Hz / 8 rows = 100.16Hz refresh rate
 // if TCNT2 = 61, ISR fires every 256 - 61 = 195 ticks
 // 125KHz / 195 = 641.026Hz / 8 rows = 80.128Hz refresh rate
  //  TCNT2 = 100;
  //TCNT2 = 61;
  TCNT2 = 150;
  if(activePanel == NULL) {
    return;
  }
  close_all_lines;  
  activePanel->run();
  activePanel->openLine(activePanel->line);
  if (++activePanel->line > 7) activePanel->line = 0;
}
#endif
