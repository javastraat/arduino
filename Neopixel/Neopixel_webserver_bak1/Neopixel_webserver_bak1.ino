#include <SPI.h>
#include <Ethernet.h>
#include <Adafruit_NeoPixel.h>

#define NUM_LIGHTS  4
#define PIN 6

int kleura=0;
int kleurb=0;
int kleurc=0;
int color=0;
int message=0;
int thisChar=0;

//define colors
#define black 0,0,0
#define blue  0,0,255
#define cyan 0,255,255
#define gray 128,128,128
#define green 0,128,0
#define lime 0,255,0
#define magenta 255,0,255
#define maroon 128,0,0
#define navy 0,0,128
#define olive 128,128,0
#define purple 128,0,128
#define red 255,0,0
#define silver 192,192,192
#define teal 0,128,128
#define white 255,255,255
#define yellow 255,255,0

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN, NEO_GRB + NEO_KHZ800);


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168, 1, 177 }; // ip in lan
byte gateway[] = { 192, 168, 1, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(80); //server port

String readString; 

//////////////////////

void setup(){

  pinMode(4, OUTPUT); //pin selected to control
  //start Ethernet
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();

  //enable serial data print 
  Serial.begin(9600); 
  Serial.println("servertest1"); // so I can keep track of what is loaded
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop(){
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100) {

          //store characters to string 
          readString += c; 
          //Serial.print(c);
        } 

        //if HTTP request has ended
        if (c == '\n') {

          ///////////////
          Serial.println(readString); //print to serial monitor for debuging 

          //now output HTML data header
             if(readString.indexOf('?') >=0) { //don't send new page
               client.println("HTTP/1.1 204 Zoomkat");
               client.println();
               client.println();  
             }
             else {
          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println();

          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<TITLE>Arduino NeoPixel Controlpage</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");

          client.println("<H1>Arduino NeoPixel Controlpage</H1>");
          client.println("<a href=\"/?off\" target=\"inlineframe\">OFF</a><br>"); 


          client.println("<a href=\"/?black\" target=\"inlineframe\">Black</a><br>");
          client.println("<a href=\"/?blue\" target=\"inlineframe\">Blue</a><br>");
          client.println("<a href=\"/?cyan\" target=\"inlineframe\">Cyan</a><br>");
          client.println("<a href=\"/?gray\" target=\"inlineframe\">Gray</a><br>");
          client.println("<a href=\"/?green\" target=\"inlineframe\">Green</a><br>");
          client.println("<a href=\"/?lime\" target=\"inlineframe\">Lime</a><br>");
          client.println("<a href=\"/?magenta\" target=\"inlineframe\">Magenta</a><br>");
          client.println("<a href=\"/?maroon\" target=\"inlineframe\">Maroon</a><br>");
          client.println("<a href=\"/?navy\" target=\"inlineframe\">Navy</a><br>");
          client.println("<a href=\"/?olive\" target=\"inlineframe\">Olive</a><br>");
          client.println("<a href=\"/?purple\" target=\"inlineframe\">Purple</a><br>");
          client.println("<a href=\"/?red\" target=\"inlineframe\">Red</a><br>");
          client.println("<a href=\"/?silver\" target=\"inlineframe\">Silver</a><br>");
          client.println("<a href=\"/?teal\" target=\"inlineframe\">Teal</a><br>");
          client.println("<a href=\"/?white\" target=\"inlineframe\">White</a><br>");
          client.println("<a href=\"/?yellow\" target=\"inlineframe\">Yellow</a><br>");


          //client.println("<IFRAME name=inlineframe src=\"res://D:/WINDOWS/dnserror.htm\" width=1 height=1\">");
          client.println("<IFRAME name=inlineframe style=\"display:none\" >");          
          client.println("</IFRAME>");

          client.println("</BODY>");
          client.println("</HTML>");
             }

          delay(1);
          //stopping client
          client.stop();

          ///////////////////// control neopixel
          
          if(readString.indexOf("off") >0)//checks for off
          {
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,black);
          strip.show();
          delay(10);
          }
      
            }
            if(readString.indexOf("black") >0)//checks for off
          {
           
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,black);
          strip.show();
          delay(10);
          }
          }
          
          if(readString.indexOf("blue") >0)//checks for off
          {
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,blue);
          strip.show();
          delay(10);
          }
          }
          
          if(readString.indexOf("cyan") >0)//checks for off
          {
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,cyan);
          strip.show();
          delay(10);
          }
          }
          
          if(readString.indexOf("gray") >0)//checks for off
          {
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,gray);
          strip.show();
          delay(10);
          }
          }
          
          if(readString.indexOf("green") >0)//checks for off
          {
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,green);
          strip.show();
          delay(10);
          }
          }
          
          if(readString.indexOf("lime") >0)//checks for off
          {
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,lime);
          strip.show();
          delay(10);
          }
          }
          
          if(readString.indexOf("magenta") >0)//checks for off
          {
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,magenta);
          strip.show();
          delay(10);
          }
          }
          
          if(readString.indexOf("maroon") >0)//checks for off
          {
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,maroon);
          strip.show();
          delay(10);
          }
          }
          
          if(readString.indexOf("navy") >0)//checks for off
          {
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,navy);
          strip.show();
          delay(10);
          }
          }
          
          if(readString.indexOf("olive") >0)//checks for off
          {
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,olive);
          strip.show();
          delay(10);
          }
          }
          
          if(readString.indexOf("purple") >0)//checks for off
          {
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,purple);
          strip.show();
          delay(10);
          }
          }
          
          if(readString.indexOf("red") >0)//checks for off
          {
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,red);
          strip.show();
          delay(10);
          }
          }
          
          if(readString.indexOf("silver") >0)//checks for off
          {
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,silver);
          strip.show();
          delay(10);
          }
          }
          
          if(readString.indexOf("teal") >0)//checks for off
          {
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,teal);
          strip.show();
          delay(10);
          }
          }
          
          if(readString.indexOf("white") >0)//checks for off
          {
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,white);
          strip.show();
          delay(10);
          }
          }
          
          if(readString.indexOf("yellow") >0)//checks for off
          {
          for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i,yellow);
          strip.show();
          delay(10);
          }
          }
          
          
          
          
          
          //clearing string for next read
          readString="";

        }
      }
    }
  }
} 


