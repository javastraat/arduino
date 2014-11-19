#ifndef _PINS_H
#define _PINS_H
#include <Arduino.h>
#include <avr/io.h>

#define PIN_LED    13

#define PIN_SCL    A5
#define PIN_SDA    A4
#define PIN_IMU_Z  A3
#define PIN_IMU_Y  A2
#define PIN_IMU_X  A1
#define PIN_LDR    A0


// 74HC595 The column-cathodes
#define PIN_DATA   4
#define PIN_LATCH  3
#define PIN_CLOCK  2
#define PORT_SHIFTOUT PORTD
#define PORT_MASK_DATA   _BV(4)
#define PORT_MASK_LATCH  _BV(3)
#define PORT_MASK_CLOCK  _BV(2)

//                   matrix pin   9 14  8 12  1  7  2  5
#define MATRIX_ANODE_ROW_PINS   {12,10, 9,11, 5, 8, 6, 7}  // (Arduino pin)

//                   matrix pin  13  3  4 10  6 11 15 16
#define MATRIX_CATHODE_COL_BITS { 5, 4, 3, 1, 0, 2, 6, 7}  // (74HC595 bit)

/*
  8x8Clock, Mark Wilson 2012
                                 +-----------------------------------+
                                 |            ARDUINO                |
                                 |                +-----------[AREF] |                       
                                 |                |            [GND] |           
                                 |                |         LED [13] |      Matrix pin (Anode, Rows)               
                                 | [RST]          |             [12] +------[ 9]               
                                 | [3V3]----------+         PWM [11] +------[12]
                                 | [5V0]                    PWM [10] +------[14]    
                                 | [GND]                    PWM [ 9] +------[ 8]    
                                 | [GND]                        [ 8] +------[ 7]                       
                                 | [Vin]                        [ 7] +------[ 5]                       
                                 |                          PWM [ 6] +------[ 2]                       
               {LDR}-------------+ [A0]                     PWM [ 5] +------[ 1]    +-----------------+          Matrix pin (Cathode, Columns)
               {Accelerometer X}-+ [A1]                         [ 4] +--------------+ [14:DS]       b0+--[R150]--[ 6]
                             {Y}-+ [A2]                     PWM [ 3] +--------------+ [12:ST]       b1+--[R150]--[10]
       +-------------+       {Z}-+ [A3]                         [ 2] +--------------+ [11:SH]       b2+--[R150]--[11]
       |       [SDA] +-----------+ [A4] SDA                  TX [ 1] |              |               b3+--[R150]--[ 4]
       |       [SCL] +-----------+ [A5] SCL                  RX [ 0] |              |     74HC595   b4+--[R150]--[ 3]
       | DS3232      |           |                                   |              |               b5+--[R150]--[13] 
       +-------------+           +-----------------------------------+              |               b6+--[R150]--[15]
                                                                                    |               b7+--[R150]--[16]
                                                                                    +-----------------+
  RTC:           www.freetronics.com/rtc
  Accelerometer: www.freetronics.com/am3x 
  LDR:           www.freetronics.com/light
*/

#endif
