//  Copyright (C) 2013 Andy Krenz
//  
//
//  This file is part of Arduino EthernetBonjour.
//
//  Ohm Lantern is free software: you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public License
//  as published by the Free Software Foundation, either version 3 of
//  the License, or (at your option) any later version.
//
//  OHM Lantern is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with OHM Lantern. If not, see
//  <http://www.gnu.org/licenses/>.
//
//


#if defined(ARDUINO) && ARDUINO > 18
#include <SPI.h>
#endif
#include <Ethernet.h>
//#include <EthernetBonjour.h>

#include <Adafruit_NeoPixel.h>
#include <colorcodes.h>
#define NUM_LIGHTS 4 //Number off connected leds 
#define PIN 6 //Pin 6 for Data

int ledPin = 9 ;               // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for a pushbutton)
int val = 0;                    // variable for reading the pin status

//int kleura=0;
//int kleurb=0;
//int kleurc=0;
//int color=0;
//int message=0;
//int thisChar=0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN, NEO_GRB + NEO_KHZ800);

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168  , 0, 177 }; // ip in lan
byte gateway[] = { 192, 168, 0, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask




// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);
String readString; 


void setup()
{
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare pushbutton as input
  pinMode(6, OUTPUT); //pin selected to control
  //start Ethernet
  Ethernet.begin(mac, ip, gateway, subnet);
  //EthernetBonjour.begin("arduino");
  //EthernetBonjour.addServiceRecord("Arduino Bonjour Webserver._http", 80,MDNSServiceTCP);
    
  server.begin();

  //enable serial data print 
  Serial.begin(9600); 
  Serial.println("Neopixel"); // so I can keep track of what is loaded
  Serial.println(Ethernet.localIP());
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}
void loop()
{
 // EthernetBonjour.run();
  buttonPush(0);
  //Serial.println("begin off loop"); // so I can keep track of what is loaded 
  // listen for incoming clients
  EthernetClient client = server.available();
  
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    String buffer = "";  //  Declare the buffer variable
    Serial.println("Houston we got a client"); // so I can keep track of what is loaded 
 
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);  //  Send every character read to serial port 
        buffer+=c;        //  Assign to the buffer

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          
          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<TITLE>Arduino NeoPixel Controlpage</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");
          client.println("<H1>Arduino NeoPixel Controlpage</H1>");
          
          client.println("<br />");
          
          // 2<-- Create a form
          client.print("<FORM action=\"/\" >");
          client.print("<INPUT type=\"radio\" name=\"color\" value=\"OFF\">OFF");
          client.println("<br />");
          client.print("<INPUT type=\"radio\" name=\"color\" value=\"white\">White");
          client.println("<br />");
          client.print("<INPUT type=\"radio\" name=\"color\" value=\"polizei\">Polizei");
          client.println("<br />");
          client.print("<INPUT type=\"radio\" name=\"color\" value=\"knightrider\">Knightrider");
          client.println("<br /><br />");
         // client.print(" Or choose color e.g. red,white,yellow etc. : <br /><INPUT type=\"text\" name=\"color\" value=\"red\">");
          //client.println("<br /><br />");
          client.print("<INPUT type=\"submit\" value=\"Submit\"> </FORM>");
          client.println("<br/>");
          
          client.println("<br />");
          
          client.println("</BODY>");
          client.println("</HTML>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
          buffer="";       //  Clear the buffer at end of line
        } else if (c == '\r') {            
          
          
//
//Special modes are rainbow,knightrider,polizei,fireplace,
//text,mail,pacman,invaders and hearts. 
//
if(buffer.indexOf("GET /?color=rainbow")>=0)
rainbowCycle(20); 

if(buffer.indexOf("GET /?color=knightrider")>=0)
kit(strip.Color(red), 100);

if(buffer.indexOf("GET /?color=polizei")>=0)
polizei(40);

if(buffer.indexOf("GET /?color=fireplace")>=0)
rainbowCycle(2); 

if(buffer.indexOf("GET /?color=text")>=0)
kit(strip.Color(white), 100);

if(buffer.indexOf("GET /?color=mail")>=0)
kit(strip.Color(green), 100);

if(buffer.indexOf("GET /?color=pacman")>=0)
kit(strip.Color(yellow), 100);

if(buffer.indexOf("GET /?color=invaders")>=0)
kit(strip.Color(green), 100);

if(buffer.indexOf("GET /?color=hearts")>=0)
kit(strip.Color(magenta), 100);


//Colors are : black,blue,cyan,gray,green,lime,magenta,maroon,navy,
//olive,purple,red,silver,teal,white,yellow and of course oranje
//extra black for command off
if(buffer.indexOf("GET /?color=OFF")>=0)
           colorSet(strip.Color(black), 200); 
if(buffer.indexOf("GET /?color=black")>=0)
            colorSet(strip.Color(black),200);
if(buffer.indexOf("GET /?color=blue")>=0)
            colorSet(strip.Color(blue),0);
if(buffer.indexOf("GET /?color=cyan")>=0)
            colorSet(strip.Color(cyan),0);
if(buffer.indexOf("GET /?color=gray")>=0)
            colorSet(strip.Color(gray),0);
if(buffer.indexOf("GET /?color=green")>=0)
            colorSet(strip.Color(green),0);
if(buffer.indexOf("GET /?color=lime")>=0)
            colorSet(strip.Color(lime),	0);
if(buffer.indexOf("GET /?color=magenta")>=0)
            colorSet(strip.Color(magenta),0);
if(buffer.indexOf("GET /?color=maroon")>=0)
            colorSet(strip.Color(maroon),0);
if(buffer.indexOf("GET /?color=navy")>=0)
            colorSet(strip.Color(navy),0);
if(buffer.indexOf("GET /?color=olive")>=0)
            colorSet(strip.Color(olive),0);
if(buffer.indexOf("GET /?color=purple")>=0)
            colorSet(strip.Color(purple),0);
if(buffer.indexOf("GET /?color=red")>=0)
            colorSet(strip.Color(red),0);
if(buffer.indexOf("GET /?color=silver")>=0)
            colorSet(strip.Color(silver),0);
if(buffer.indexOf("GET /?color=teal")>=0)
            colorSet(strip.Color(teal),0);
if(buffer.indexOf("GET /?color=white")>=0)
            colorSet(strip.Color(white),0);
if(buffer.indexOf("GET /?color=yellow")>=0)
            colorSet(strip.Color(yellow),0);
if(buffer.indexOf("GET /?color=oranje")>=0)
            colorSet(strip.Color(oranje),0);
 
 
 
 
 
 
          }
        else {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
 //Serial.println("end of loop"); // so I can keep track of what is loaded 
 
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();  
  }
}


//
//Special funtions
//Colorset
//Special Modes
//
//
// Function
//
// Button function
void buttonPush(uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
val = digitalRead(inputPin);  // read input value
  if (val == LOW) {            // check if the input is HIGH
    digitalWrite(ledPin, LOW);  // turn LED OFF
  } else {
    digitalWrite(ledPin, HIGH); // turn LED ON
    Serial.println("buttonPush"); // so I can keep track of what is loaded 
    rainbowCycle(10);
  }
// Serial.println("Buttoncheck"); // so I can keep track of what is loaded 
      delay(wait);
  }
}
//
// Colorwheel function used for rainbowcycle
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
//
//
//Cycloneeye used for polizei (need to be extracted)
//
void CylonEyeUp(uint32_t Co, uint32_t Ct, uint32_t Ctt, uint8_t Delay, int TotalPixels, int pStart) {
  for(int i=pStart; i<TotalPixels; i++) {
    strip.setPixelColor(i+2, Ctt);  //Third Dot Color
    strip.setPixelColor(i+1, Ct);   //Second Dot Color
    strip.setPixelColor(i, Co);     //Center Dot Color
    strip.setPixelColor(i-1, Ct);   //Second Dot Color
    strip.setPixelColor(i-2, Ctt);  //Third Dot Color

    strip.setPixelColor(i-3, strip.Color(0,0,0)); //Clears the dots after the 3rd color
    strip.show();
    //Serial.println(i); //Used For pixel Count Debugging
    delay(Delay);

  }
}
void CylonEyeDown(uint32_t Co, uint32_t Ct, uint32_t Ctt, uint8_t Delay, int TotalPixels, int pEnd) {
  for(int i=TotalPixels-1; i>pEnd; i--) {
    strip.setPixelColor(i-2, Ctt);  //Third Dot Color
    strip.setPixelColor(i-1, Ct);   //Second Dot Color
    strip.setPixelColor(i, Co);    //Center Dot Color
    strip.setPixelColor(i+1, Ct);  //Second Dot Color
    strip.setPixelColor(i+2, Ctt); //Third Dot Color

    strip.setPixelColor(i+3, strip.Color(0,0,0)); //Clears the dots after the 3rd color
    strip.show();
    //Serial.println(i); //Used For pixel Count Debugging
    delay(Delay);
  }



}



//
//
//
// Fill all leds with plain color
void colorSet(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

//
//Special modes
//
// special effects rainbow with wheel for glow r-g-b-g-r
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*10; j++) { // 10 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
    // Turn them off
    for( int i = 0; i<NUM_LIGHTS; i++){
        strip.setPixelColor(i, black);
        strip.show();
    }

}


//special effect kit/knightrider
void kit(uint32_t c, uint8_t wait) {
  uint16_t j; 
  for(j=0; j<20; j++) {
       for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      if (i > 0) {
        strip.setPixelColor(i-1, strip.Color(0, 0, 0));
      }
      strip.show();
      delay(wait);
  }
  for(uint16_t i=strip.numPixels() - 1; i > 0; i--) {
      strip.setPixelColor(i, c);
      if (i < strip.numPixels() - 1) {
        strip.setPixelColor(i+1, strip.Color(0, 0, 0));
      }
      strip.show();
      delay(wait);
  }
}
    // Turn them off
    for( int i = 0; i<NUM_LIGHTS; i++){
        strip.setPixelColor(i, black);
        strip.show();
    }
}


void polizei(uint8_t wait) {
 //Example: CylonEyeUp(Center_Dot_Color, Second_Dot_color, Third_Dot_color, wait_T, PixelCount, Pixel_Start_End);
  for(int j=0; j<20; j++) {
  CylonEyeUp(strip.Color(175,0,0), strip.Color(25,0,0), strip.Color(10,0,0), 40, 4, 0);
  delay(wait);
  //Example: CylonEyeDown(Center_Dot_Color, Second_Dot_color, Third_Dot_color, wait_T, PixelCount, Pixel_Start_End);
  CylonEyeDown(strip.Color(0,0,175), strip.Color(0,0,25), strip.Color(0,0,10), 40, 4 ,0);
  delay(wait);
  } 
  // Turn them off
    for( int i = 0; i<NUM_LIGHTS; i++){
        strip.setPixelColor(i, black);
        strip.show();
    }}


// Fill all leds with plain color
void fireplace(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


// Fill all leds with plain color
void text(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


// Fill all leds with plain color
void mail(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


// Fill all leds with plain color
void pacman(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


// Fill all leds with plain color
void invaders(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


// Fill all leds with plain color
void hearts(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}










void knightRider() {
    int loops = 5;
    int count = 0;
    int timer = 20;
  
    uint32_t low = strip.Color(0,0,0);
    uint32_t high = strip.Color(0,255,0);
  
    // Flash Lights
    for(int i = 0; i < loops; i++){
        for (count=0;count<NUM_LIGHTS;count++) {
            strip.setPixelColor(count, high);
            strip.show();
            delay(timer);
            strip.setPixelColor(count+1,high);
            strip.show();
            delay(timer);
            strip.setPixelColor(count, low);
            strip.show();
            delay(timer*2);
        }
        for (count=NUM_LIGHTS;count>0;count--) {
            strip.setPixelColor(count, high);
            strip.show();
            delay(timer);
            strip.setPixelColor(count-1, high);
            strip.show();
            delay(timer);
            strip.setPixelColor(count, low);
            strip.show();
            delay(timer*2);
        }
    }
    
    // Turn them off
    for( int i = 0; i<NUM_LIGHTS; i++){
        strip.setPixelColor(i, low);
        strip.show();
    }
}

