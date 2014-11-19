//#include <SPI.h>
//#include <HITBmirf.h>
#include <PRESENTasm.h>

#include "HITBadge.h"
#include "PRESENTapi.h"

#define DEBUG

//crypto vars
static unsigned char encryption[24], key[16] = {0x61, 0x64, 0x63, 0x66, 0x31, 0x37, 0x65, 0x32, 0x66, 0x65, 0x38, 0x33, 0x65, 0x64, 0x61, 0x31};
char print_buf[18];

//general vars
unsigned long time;
//HITBadge badge;

/// SETUP
void setup() { 
  
  // initialise SPI driver for nrf module
//  HITBmirf.init();
  // set receiving address
//  HITBmirf.setRADDR((byte *)"clie1");
  // writes channel, payload size and rec.address to nrf
//  HITBmirf.config();
  
  // init serial connection
  Serial.begin(9600);  
  Serial.println("Listening..."); 
  
  // crypto setup
  memset(encryption, 0x0, 8);
  memcpy(encryption+8, key, 16); //kopieert de key naar memory, direct achter encryption.
  test_print();
}

/// PROGRAM
void loop() {

  // A 32 byte buffer to store the data.
  byte data[32]; //  HITBmirf.payload];
  for(int i=0; i<32; i++)data[i] = i; //incremented data
  
  /*
   * If a packet has been received.
   *
   * isSending also restores listening mode when it 
   * transitions from true to false.
   */ 
/*  
  if(!HITBmirf.isSending() && HITBmirf.dataReady()){
    Serial.println("Got packet");
    
    HITBmirf.getData((byte *)&data);
*/
    #ifdef DEBUG 
    Serial.println("Data received:");
    printData(data);
    Serial.println("=============================\n=============================");
    #endif
    
    //encrypt data
    for(int i=0; i<4; i++){
      memset(encryption, 0x0, 8);
      int offset = i*8;   
      for(int d=0; d<8; d++){ 
        //Serial.print(data[offset+d]);
        //Serial.println();
        encryption[d] = data[offset+d]; 
        if(d==7){   
          Serial.print("Plain");          
          test_print();     
          do_encryption(encryption);
          Serial.print("Crypto");          
          test_print();
          do_decryption(encryption);
          Serial.print("Decrypto");
          test_print();          
          memcpy(data+offset, encryption, 8);
        }
      }
      Serial.println("Encrypt per byte:");
      printData(data);
    }
   
    //#ifdef DEBUG 
    //Serial.println("Data encrypted:");
    //printData();
    //#endif
/*   
    // Set the send-to address.          
    HITBmirf.setTADDR((byte *)"serv1");        
    // Send the data back to the client.     
    HITBmirf.send(data);
    
    Serial.println("** Sent ACK **");
    Serial.println();
*/    
    for(int i=0; i<3; ++i){
      delay(1000);
      //badge.knightRider();
    }
    //badge.reset();
  }
//}

void printData(byte* data){
    Serial.println("Data:");
    for(int i=0; i<32; i++){ //HITBadge.payload
      byte tmp = data[i];
      Serial.print(tmp, HEX);
      Serial.print("\t");
      if(i%4 == 3) Serial.println();
    } 
}

void test_print(){
	Serial.println(" buffer:");
	sprintf(print_buf, "%02x%02x%02x%02x %02x%02x%02x%02x", encryption[0], encryption[1], encryption[2], encryption[3], encryption[4], encryption[5], encryption[6], encryption[7]);
	Serial.println(print_buf);
}
