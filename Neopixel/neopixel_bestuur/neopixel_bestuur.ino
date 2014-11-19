#include <Adafruit_NeoPixel.h>
#define NUM_LIGHTS  4
#define PIN 6
int kleura=0;
int kleurb=0;
int kleurc=0;
int kleur=0;
int message=0;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);  //set serial to 9600 baud rate
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  //colorWipe(strip.Color(255, 0, 0), 50); // Red
  //colorWipe(strip.Color(0, 255, 0), 50); // Green
  //colorWipe(strip.Color(0, 0, 255), 50); // Blue
  //colorWipe(strip.Color(255, 255, 255), 50); // White
  //rainbow(20);
  //rainbowCycle(20);
  //chase(strip.Color(0,0,255), 50);
  //kit(strip.Color(0,0,255), 50);
  //kColorWipe(5, strip.Color(0, 255, 0), 50); // Green  
  //kColorWipe(5, strip.Color(0, 0, 255), 50); // Blue

 if (Serial.available() > 0) { //  Check if there is a new message
      message = Serial.read();    //  Put the serial input into the message

   if (message == 'r'){  //  If a capitol R is received...
     kleura = 255;       //  Set redLED to 255 (on)
     kleurb = 0;       //  Set redLED to 255 (on)
     kleurc = 0;       //  Set redLED to 255 (on)
   }
   if (message == 'g'){  //  If a lowercase r is received...
     kleura = 0;         //  Set redLED to 0 (off)
     kleurb = 255;       //  Set redLED to 255 (on)
     kleurc = 0;       //  Set redLED to 255 (on)
   }   
   if (message == 'b'){  //  If a lowercase r is received...
     kleura = 0;         //  Set redLED to 0 (off)
     kleurb = 0;       //  Set redLED to 255 (on)
     kleurc = 255;       //  Set redLED to 255 (on)
   }
   if (message == 'x'){  //  If a lowercase r is received...
     kleura = 0;         //  Set redLED to 0 (off)
     kleurb = 0;         //  Set redLED to 0 (off)
     kleurc = 0;         //  Set redLED to 0 (off)
   }
   if (message == 'w'){  //  If a lowercase r is received...
     kleurb = 255;         //  Set redLED to 0 (off)
     kleurb = 255;         //  Set redLED to 0 (off)
     kleurc = 255;         //  Set redLED to 0 (off)
   }
//kColorWipe(5, strip.Color(kleura,kleurb,kleurc), 50); // Blue



  uint32_t kleur = strip.Color(kleura,kleurb,kleurc);
  
    // Turn them on
   for( int i = 0; i<NUM_LIGHTS; i++){
       strip.setPixelColor(i, kleur);
       strip.show(); 



  
    }}
  


}



