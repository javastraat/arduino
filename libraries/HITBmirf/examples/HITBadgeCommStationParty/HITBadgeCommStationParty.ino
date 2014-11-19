#include <SPI.h>
#include <HITBmirf.h>
#include <string.h>

#include "HITBadge.h"

#define DEBUG;

unsigned long time;
HITBadge badge;
char print_buf[7];

String NAMES[2]={ // string array with first 5 chars of nick
"fish1",
"ardil"
};
char charbuf[6];

static unsigned char buffer[7], nick[5];
byte location= 0x8;

char MAC[]="send1";

/// SETUP
void setup() { 
  
  // initialise SPI driver for nrf module
  HITBmirf.init();
  // set receiving address
  HITBmirf.setRADDR((byte *)MAC);
  // writes channel, payload size and rec.address to nrf
  HITBmirf.config();
  
  // init serial connection
  //Serial.begin(9600);  
  //Serial.println("Listening...");
  //Serial.println(); 
}

/// PROGRAM
void loop() {
  delay(2);
  for(int i=0; i<=64; i++){
    NAMES[i].toCharArray(charbuf, 6);
    //Serial.println(charbuf);
    sendMessageParty((char*) charbuf, location);
    delay(5);
    //if(i==71)
    //    asm volatile(" jmp 0");
  }     

  
}

void sendMessageParty(char address[], byte location){
  // craft the 32 bytes payload with data and zeros
  byte data[HITBmirf.payload];
  // fill data frame with zeros
  for(int i=0; i<HITBmirf.payload; i++){
    if(i<5)
      data[i] = 0x0;
    else if(i<10)
      data[i]= address[i-5];
    else if(i<11)
      data[i] = location;  
    else
      data[i] = 0x00;
  }
      
  /*
   * Pretty printing of data, seperated by byte
   * Works only if "#define DEBUG" is not commented
   */
  #ifdef DEBUG
  Serial.println("Data out: ");
  for(int i=0; i<HITBmirf.payload; i++){
    Serial.print(data[i], HEX);
    Serial.print("\t");
    if(i%4 == 3) Serial.println();
  }
  #endif
  
  // Set the send-to address.           
  HITBmirf.setTADDR((byte*) address);    
  // Send data to the receiver 
  HITBmirf.send((byte *)&data);
  
  while(HITBmirf.isSending()){
    // empty loop while sending
  }
  // Sending succeeded
  //badge.circleR();
  Serial.println("** Finished sending **");
  delay(10);
  while(!HITBmirf.dataReady()){
    if ( ( millis() - time ) > 1000 ) {
      Serial.println();
      //Serial.println("Timeout on response from server!");
      badge.reset();
      return; // return if Timeout occurs
    }
  }

  // Load the packet into the buffer. 
  HITBmirf.getData((byte *)&data);
  
  // Print RTT
  Serial.print("Round-trip time: ");
  Serial.print((millis() - time));
  Serial.println(" ms");   
  
 // badge.circleL();
  badge.reset();
}




