/*
 Chat  Server
 
 A simple server that distributes any incoming messages to all
 connected clients.  To use telnet to  your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 Using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <Adafruit_NeoPixel.h>
#define NUM_LIGHTS  4
#define PIN 6
int kleura=0;
int kleurb=0;
int kleurc=0;
int kleur=0;
int message=0;
int thisChar=0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN, NEO_GRB + NEO_KHZ800);

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1, 177);
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 0, 0);


// telnet defaults to port 23
EthernetServer server(23);
boolean alreadyConnected = false; // whether or not the client was connected previously

void setup() {
  // initialize the ethernet device
  Ethernet.begin(mac, ip, gateway, subnet);
  // start listening for clients
  server.begin();
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  


  Serial.print("Chat server address:");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();

  // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clead out the input buffer:
      client.flush();    
      Serial.println("We have a new client");
      client.println("Hello, client!"); 
      client.println("b=blue,w=white,g=green"); 
      client.println("y=yellow,r=red,x=off"); 
      client.println("k=kit,R=rainbow,c=chase");
      client.println("s=stoplight");  
      alreadyConnected = true;
    
    }

 if (client.available() > 0) { //  Check if there is a new message

      char thisChar = client.read();
      // echo the bytes back to the client:
      server.write(thisChar);
      // echo the bytes to the server as well:
      Serial.write(thisChar);
     

    if (thisChar == 'w'){
      strip.setPixelColor(0, 255,255,255);
      strip.setPixelColor(1, 255,255,255);
      strip.setPixelColor(2, 255,255,255);
      strip.setPixelColor(3, 255,255,255);
      strip.show();
    
    }
    if (thisChar == 'r'){
      strip.setPixelColor(0, 255,0,0);
      strip.setPixelColor(1, 255,0,0);
      strip.setPixelColor(2, 255,0,0);
      strip.setPixelColor(3, 255,0,0);
      strip.show();
     
    }

        if (thisChar == 'b'){
      strip.setPixelColor(0, 0,0,255);
      strip.setPixelColor(1, 0,0,255);
      strip.setPixelColor(2, 0,0,255);
      strip.setPixelColor(3, 0,0,255);
      strip.show();
     
    }
    
        if (thisChar == 'g'){
      strip.setPixelColor(0, 0,255,0);
      strip.setPixelColor(1, 0,255,0);
      strip.setPixelColor(2, 0,255,0);
      strip.setPixelColor(3, 0,255,0);
      strip.show();
     
    }
    
        if (thisChar == 'y'){
      strip.setPixelColor(0, 255,255,0);
      strip.setPixelColor(1, 255,255,0);
      strip.setPixelColor(2, 255,255,0);
      strip.setPixelColor(3, 255,255,0);
      strip.show();
     
    }
    
      if (thisChar == 'x'){
      strip.setPixelColor(0, 0,0,0);
      strip.setPixelColor(1, 0,0,0);
      strip.setPixelColor(2, 0,0,0);
      strip.setPixelColor(3, 0,0,0);
      strip.show();
     
    }
      if (thisChar == 's'){
      strip.setPixelColor(0, 255,0,0);
      strip.setPixelColor(1, 255,255,0);
      strip.setPixelColor(2, 0,255,0);
      strip.setPixelColor(3, 0,0,255);
      strip.show();
     
    }

    if (thisChar == 'k'){kit(strip.Color(0,0,255), 100);
    }
    if (thisChar == 'R'){rainbowCycle(20);
    }
    if (thisChar == 'c'){chase(strip.Color(0,0,255), 100);
    }

  
}}
  
}

     
     
     
     
  


void kit(uint32_t c, uint8_t wait) {
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

void chase(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      if (i) strip.setPixelColor(i-1, strip.Color(0, 0, 0));
      strip.show();
      delay(wait);
  }
}

// Fill every kth dots one after the other with a color
void kColorWipe(uint8_t k, uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i+=k) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

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
