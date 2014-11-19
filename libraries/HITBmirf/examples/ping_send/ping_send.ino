/**************************************************************\
 A HITBmirf sender which sends a maximal frame of 32 bytes.
 The last byte contains a time-stamp.
 If the receiver runs the ping_receive program  it will send the
 data frame back
\**************************************************************/

#include <SPI.h>
#include <HITBmirf.h>

#define DEBUG

void setup(){

  // initialise SPI driver for nrf module
  HITBmirf.init();
  // set receiving address
  HITBmirf.setRADDR((byte *)"clie1");
  // writes channel, payload size and rec.address to nrf
  HITBmirf.config();

  // init serial connection  
  Serial.begin(9600);
  Serial.println("Beginning ... "); 
}

void loop(){
  delay(5000);
  unsigned long time = millis();
    
  // empty line, makes output more readable
  Serial.println();
  
  // make a byte array of 32 bytes (default HITBmirf payload size)
  byte data[HITBmirf.payload];
  // fill data frame with zeros
  for(int i=0; i<HITBmirf.payload; i++)data[i] = 0x0000;
  
  // the last four bytes of our data frame is the timestamp  
  for(int i=0; i<sizeof(time); i++){ 
    byte tmp = (time >> (8*i)) & 0xFF ; 
    data[(HITBmirf.payload-1)-i] = tmp;
  }
    
  /*
   * Pretty printing of data, seperated by byte
   * Works only if "#define DEBUG" is not commented
   */
  #ifdef DEBUG
  Serial.println("Data out: ");
  for(int i=0; i<HITBmirf.payload; i++){
    byte tmp = data[i];
    Serial.print(tmp, BIN);
    Serial.print("\t\t");
    if(i%4 == 3) Serial.println();
  }
  #endif
  
  // Set the send-to address.           
  HITBmirf.setTADDR((byte *)"serv1");  
  // Send data to the receiver 
  HITBmirf.send((byte *)&data);
  
  while(HITBmirf.isSending()){
    // empty loop while sending
  }
  
  Serial.println("** Finished sending **");
  delay(10);
  while(!HITBmirf.dataReady()){
    if ( ( millis() - time ) > 1000 ) {
      Serial.println("Timeout on response from server!");
      return; // return if Timeout occurs
    }
  }

  // Load the packet into the buffer. 
  HITBmirf.getData((byte *)&data);
  // Print RTT
  Serial.print("Round-trip time: ");
  Serial.print((millis() - time));
  Serial.println(" ms");
  
} 
  
  
  
