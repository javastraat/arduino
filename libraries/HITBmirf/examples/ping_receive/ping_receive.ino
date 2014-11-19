/**************************************************************\
 A HITBmirf receiver which blinks led 5 if packet is received.
 It will also send back the data it received as a form of ACK.
\**************************************************************/

#include <SPI.h>
#include <HITBmirf.h>

#define DEBUG

void setup(){
  
  // pin 5 has a LED attached
  pinMode(5, OUTPUT);

  // initialise SPI driver for nrf module
  HITBmirf.init();
  // set receiving address
  HITBmirf.setRADDR((byte *)"serv1");
  // writes channel, payload size and rec.address to nrf
  HITBmirf.config();

  // init serial connection
  Serial.begin(9600);  
  Serial.println("Listening..."); 
}

void loop(){
  
  // A 32 byte buffer to store the data.
  byte data[HITBmirf.payload];
  
  /*
   * If a packet has been received.
   *
   * isSending also restores listening mode when it 
   * transitions from true to false.
   */   
  if(!HITBmirf.isSending() && HITBmirf.dataReady()){
    Serial.println("Got packet");
    
    // Load the packet into the buffer. 
    HITBmirf.getData((byte *)&data);
    
    /*
     * Pretty printing of data, seperated by byte
     * Works only if "#define DEBUG" is not commented
     */
    #ifdef DEBUG 
    Serial.println("Data received:");
    for(int i=0; i<HITBmirf.payload; i++){
      byte tmp = data[i];
      Serial.print(tmp, BIN);
      Serial.print("\t\t");
      if(i%4 == 3) Serial.println();
    }
    #endif
   
    // Set the send-to address.          
    HITBmirf.setTADDR((byte *)"clie1");        
    // Send the data back to the client.     
    HITBmirf.send(data);
    
    Serial.println("** Sent ACK **");
    Serial.println();
    // Blink led 5 a couple times
    for(int i=0; i < 6; i++){
      digitalWrite(5, HIGH);
      delay(50);
      digitalWrite(5, LOW);
      delay(50);
    }  
  }
}
