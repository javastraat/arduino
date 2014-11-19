// - - - - -
// DmxSerial - A hardware supported interface to DMX.
// DmxSerialRecv.ino: Sample DMX application for retrieving 3 DMX values:
// address 1 (red) -> PWM Port 9
// address 2 (green) -> PWM Port 6
// address 3 (blue) -> PWM Port 5
// 
// Copyright (c) 2011 by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// 
// Documentation and samples are available at http://www.mathertel.de/Arduino
// 25.07.2011 creation of the DmxSerial library.
// 10.09.2011 fully control the serial hardware register
//            without using the Arduino Serial (HardwareSerial) class to avoid ISR implementation conflicts.
// 01.12.2011 include file and extension changed to work with the Arduino 1.0 environment
// 28.12.2011 changed to channels 1..3 (RGB) for compatibility with the DmxSerialSend sample.
// 10.05.2012 added some lines to loop to show how to fall back to a default color when no data was received since some time.
// - - - - -

#include <DMXSerial.h>
#include <Adafruit_NeoPixel.h>
// Constants for demo program

#define RedDefaultLevel   255
#define GreenDefaultLevel 0
#define BlueDefaultLevel  0
Adafruit_NeoPixel strip = Adafruit_NeoPixel(300, 6, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  DMXSerial.init(DMXReceiver);
  
  // set some default values
  //DMXSerial.write(1,25);
  //DMXSerial.write(2,0);
  //DMXSerial.write(3,0);
  
}


void loop() {
runtime (strip.Color(25, 0, 0), 0);
}
void runtime(uint32_t c, uint8_t wait) {

  // Calculate how long no data backet was received
  unsigned long lastPacket = DMXSerial.noDataSince();
  
  if (lastPacket < 500) {
    // read recent DMX values and set pwm levels 
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, (DMXSerial.read(1),DMXSerial.read(2),DMXSerial.read(3)));
      strip.show();
      delay(50);
  
  }} else {
    // Show pure red color, when no data was received since 5 seconds or more.
       for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(50);
   
  
  } // if
}
// End.
}
