#include <SPI.h>
#include <HITBmirf.h>
#include <string.h>

#include "HITBadge.h"

//#define DEBUG

unsigned long time;
HITBadge badge;
char print_buf[7];

static unsigned char buffer[7], nick[5];
byte location;

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
  Serial.begin(9600);  
  Serial.println("Listening...");
  Serial.println(); 
}

/// PROGRAM
void loop() {
  
  unsigned long time = millis();
  while (Serial.available() > 0){
    memset(buffer, 0x0, 7);
    Serial.readBytesUntil('\n', (char*) buffer, 7); //was 8
    if(buffer[5] != 0x40) //fallback als serial input niet klopt
      return;
    printBuffer();      
    splitString();
    //Doe die shit
    sendMessage((char*) nick, location, time);
  }     
}

void sendMessage(char address[], byte location, unsigned long time){
  // craft the 32 bytes payload with data and zeros
  byte data[HITBmirf.payload];
  // fill data frame with zeros
  for(int i=0; i<HITBmirf.payload; i++){
    if(i<5)
      data[i] = MAC[i];
    else if(i<10)
      data[i] = buffer[i-5];
    else if(i<11)
      data[i] = location;
    else
      data[i] = 0x00;
  } 
  
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
  badge.circleR();
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
  
  #ifdef DEBUG
  Serial.println("Data in: ");
  for(int i=0; i<HITBmirf.payload; i++){
    Serial.print(data[i], HEX);
    Serial.print("\t");
    if(i%4 == 3) Serial.println();
  }
  #endif
  
  // Print RTT
  Serial.print("Round-trip time: ");
  Serial.print((millis() - time));
  Serial.println(" ms");   
  
  badge.circleL();
  badge.reset();
}

void printBuffer(){
  Serial.println("Serial input echo:");
  for(int i=0; i<sizeof(buffer); i++){
    Serial.print(buffer[i], HEX);
    Serial.print(' ');
  }
  Serial.println();  
}



void splitString(){
  for(int i=0; i<sizeof(buffer)-1; i++){
    if(i<5)
      nick[i]= buffer[i];    
    else
      location = buffer[i+1]-'0';    
  }
}


