#include <SPI.h>
#include <HITBmirf.h>

#include "HITBadge.h"

//#define DEBUG
#define OPERATIONAL

unsigned long time;
HITBadge badge;
char nick[]="ardil"; //only first 5 chars of nickname
char sender[]="     ";
char receiver[]="     ";
byte location;
char print_buf[12];

/// SETUP
void setup() { 
  
  // initialise SPI driver for nrf module
  HITBmirf.init();
  // set receiving address
  HITBmirf.setRADDR((byte *)nick);
  // writes channel, payload size and rec.address to nrf
  HITBmirf.config();
  
  // init serial connection
  Serial.begin(9600);  
  Serial.println("Listening...");
  Serial.println(); 
}

/// PROGRAM
void loop() {
  
  unsigned long time = millis();
  // A 32 byte buffer to store the data.
  byte data[HITBmirf.payload]; 
  for(int i=0; i<HITBmirf.payload; i++)data[i] = 0x0; //incremented data
  
  /*
   * If a packet has been received.
   *
   * isSending also restores listening mode when it 
   * transitions from true to false.
   */   
  if(!HITBmirf.isSending() && HITBmirf.dataReady()){
    Serial.println("Got packet");
    
    HITBmirf.getData((byte *)&data);
    
    #ifdef DEBUG 
    Serial.println("Data received:");
    printData(data);
    Serial.println("=============================");
    #endif  
   
    //extract data from packet......
    extractData(data);
    
    if(strcmp(nick, receiver) == 0){
      /*
       * NO ACK IS SENT
       *
      // Set the send-to address.          
      HITBmirf.setTADDR((byte *)sender);        
      // Send the data back to the sender.
      byte ack[5];
      for(int i=0; i<5; i++)data[i] = nick[i];    
      HITBmirf.send(ack);
    
      Serial.println("** Sent ACK **");
      Serial.println();
      */
      switch( location){
        case 0:
          while(true)      
            badge.fadeAll();
        case 1:
          while(true)
            badge.blinkOne();
        case 2:
          while(true)
            badge.blinkTwo();          
        case 3:
          while(true)
            badge.blinkThree();
        case 4:
          while(true)
            badge.flashAll();
        case 5:
          while(true)
            badge.circle();
        case 6:
          while(true)
            badge.knightRider();
        case 7:
          while(true)
            badge.arrow();
        case 8:
          for(int i=0; i<3; i++)     
            badge.flashAll(50);
          badge.reset();  
          return;
        case 9:
           for(int i=0; i<3; i++)
             badge.knightRider(2);
           badge.reset();  
           return;
        default: 
          badge.reset(); 
      }
    }
  }
}

void printData(byte* data){
    Serial.println("Data:");
    for(int i=0; i<HITBmirf.payload; i++){ //HITBadge.payload
      byte tmp = data[i];
      Serial.print(tmp, HEX);
      Serial.print("\t");
      if(i%4 == 3) Serial.println();
    } 
}

void extractData(byte* data){
  //byte location;
  for(int i=0; i<HITBmirf.payload; i++){
    if(i<5)
      sender[i] = data[i];
    else if(i<10)
      receiver[i-5] = data[i];
    else if(i<11)
      location = data[i];
  }            
  #ifdef OPERATIONAL   
  sprintf(print_buf, "%s wants %s to got to location %d", sender, receiver, location);
  Serial.println(print_buf);
  #endif
}

String charArrayToString(char arr[]){
  return String(arr);
}
