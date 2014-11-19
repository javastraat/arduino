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
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(10,0,20, 100);
IPAddress gateway(10,0,20, 1);
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
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


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
      alreadyConnected = true;
    } 

    if (client.available() > 0) {
      // read the bytes incoming from the client:
      char thisChar = client.read();
      // echo the bytes back to the client:
      server.write(thisChar);
      // echo the bytes to the server as well:
      Serial.write(thisChar);
     }
     if (thisChar == 'r'){  //  If a capitol R is received...
     kleura = 255;       //  Set redLED to 255 (on)
     kleurb = 0;       //  Set redLED to 255 (on)
     kleurc = 0;       //  Set redLED to 255 (on)
   }
   if (thisChar == 'g'){  //  If a lowercase r is received...
     kleura = 0;         //  Set redLED to 0 (off)
     kleurb = 255;       //  Set redLED to 255 (on)
     kleurc = 0;       //  Set redLED to 255 (on)
   }   
   if (thisChar == 'b'){  //  If a lowercase r is received...
     kleura = 0;         //  Set redLED to 0 (off)
     kleurb = 0;       //  Set redLED to 255 (on)
     kleurc = 255;       //  Set redLED to 255 (on)
   }
   if (thisCar == 'x'){  //  If a lowercase r is received...
     kleura = 0;         //  Set redLED to 0 (off)
     kleurb = 0;         //  Set redLED to 0 (off)
     kleurc = 0;         //  Set redLED to 0 (off)
   }
   if (thisChar == 'w'){  //  If a lowercase r is received...
     kleurb = 255;         //  Set redLED to 0 (off)
     kleurb = 255;         //  Set redLED to 0 (off)
     kleurc = 255;         //  Set redLED to 0 (off)
   }



  uint32_t kleur = strip.Color(kleura,kleurb,kleurc);
  
    // Turn them on
   for( int i = 0; i<NUM_LIGHTS; i++){
       strip.setPixelColor(i, kleur);
       strip.show(); 



  
    }}
  


     
     
     
     
  }}}
}



