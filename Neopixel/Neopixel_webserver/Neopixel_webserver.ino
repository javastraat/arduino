#include <SPI.h>
#include <Ethernet.h>
#include <Adafruit_NeoPixel.h>
#include <colorcodes.h>
#define NUM_LIGHTS 4 //Number off connected leds 
#define PIN 6 //Pin 6 for Data

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN, NEO_GRB + NEO_KHZ800);


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168, 1, 177 }; // ip in lan
byte gateway[] = { 192, 168, 1, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(80); //server port

String readString; 

//////////////////////

void setup(){

  //start Ethernet
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();

  //enable serial data print 
  Serial.begin(9600); 
  Serial.println("Neopixel"); // so I can keep track of what is loaded
  Serial.println(Ethernet.localIP());
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
               client.println("HTTP/1.1 204 NEOPIXEL"); 
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

          //client.println("<a href=\"/?off\" target=\"inlineframe\">Black</a> - ");
          client.println("<a href=\"/?blue\" target=\"inlineframe\">Blue</a> - ");
          client.println("<a href=\"/?cyan\" target=\"inlineframe\">Cyan</a> - ");
          client.println("<a href=\"/?gray\" target=\"inlineframe\">Gray</a> - ");
          client.println("<a href=\"/?green\" target=\"inlineframe\">Green</a> - ");
          client.println("<a href=\"/?lime\" target=\"inlineframe\">Lime</a> - ");
          client.println("<a href=\"/?magenta\" target=\"inlineframe\">Magenta</a> - ");
          client.println("<a href=\"/?maroon\" target=\"inlineframe\">Maroon</a> - ");
          client.println("<a href=\"/?navy\" target=\"inlineframe\">Navy</a> - ");
          client.println("<a href=\"/?oranje\" target=\"inlineframe\">Orange</a> - ");
          client.println("<a href=\"/?olive\" target=\"inlineframe\">Olive</a> - ");
          client.println("<a href=\"/?purple\" target=\"inlineframe\">Purple</a> - ");
          client.println("<a href=\"/?red\" target=\"inlineframe\">Red</a> - ");
          client.println("<a href=\"/?silver\" target=\"inlineframe\">Silver</a> - ");
          client.println("<a href=\"/?teal\" target=\"inlineframe\">Teal</a> - ");
          client.println("<a href=\"/?white\" target=\"inlineframe\">White</a> - ");
          client.println("<a href=\"/?yellow\" target=\"inlineframe\">Yellow</a>");

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
          colorSet(strip.Color(black), 10); 
          }
          
          if(readString.indexOf("oranje") >0)//checks for off
          {
          colorSet(strip.Color(oranje), 10); 
          }
          
          if(readString.indexOf("blue") >0)//checks for off
          {
          colorSet(strip.Color(blue), 10); 
          }
          
          if(readString.indexOf("cyan") >0)//checks for off
          {
          colorSet(strip.Color(cyan), 10); 
          }
          
          if(readString.indexOf("gray") >0)//checks for off
          {
          colorSet(strip.Color(gray), 10); 
          }
          
          if(readString.indexOf("green") >0)//checks for off
          {
          colorSet(strip.Color(green), 10); 
          }
          
          if(readString.indexOf("lime") >0)//checks for off
          {
          colorSet(strip.Color(lime), 10); 
          }
          
          if(readString.indexOf("magenta") >0)//checks for off
          {
          colorSet(strip.Color(magenta), 10); 
          }
          
          if(readString.indexOf("maroon") >0)//checks for off
          {
          colorSet(strip.Color(maroon), 10); 
          }
          
          if(readString.indexOf("navy") >0)//checks for off
          {
          colorSet(strip.Color(navy), 10); 
          }
          
          if(readString.indexOf("olive") >0)//checks for off
          {
          colorSet(strip.Color(olive), 10); 
          }
          
          if(readString.indexOf("purple") >0)//checks for off
          {
          colorSet(strip.Color(purple), 10); 
          }
          
          
          if(readString.indexOf("red") >0)//checks for off
          {
          colorSet(strip.Color(red), 10); 
          }
          
          if(readString.indexOf("silver") >0)//checks for off
          {
          colorSet(strip.Color(silver), 10); 
          }
          
          if(readString.indexOf("teal") >0)//checks for off
          {
          colorSet(strip.Color(teal), 10); 
          }
          
          if(readString.indexOf("white") >0)//checks for off
          {
          colorSet(strip.Color(white), 10); 
          }
          
          if(readString.indexOf("yellow") >0)//checks for off
          {
          colorSet(strip.Color(yellow), 10); 
          }
                    
          
          //clearing string for next read
          readString="";

        }
      }
    }
  }
} 


// Fill the dots one after the other with a color
void colorSet(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

