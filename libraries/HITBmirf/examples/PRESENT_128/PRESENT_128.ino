#include <PRESENTasm.h>
#include "PRESENTapi.h"

static unsigned char encryption[24], key[16] = {0x61, 0x64, 0x63, 0x66, 0x31, 0x37, 0x65, 0x32, 0x66, 0x65, 0x38, 0x33, 0x65, 0x64, 0x61, 0x31};
char print_buf[18];

void setup() {
	Serial.begin(9600);
	Serial.println("PRESENT-128 encrypter");
	memset(encryption, 0x0, 8);
	memcpy(encryption+8, key, 16);
        test_print();
}
void test2(){
       Serial.println("Plain:");
       for(int i=0; i<8; ++i){
         Serial.print(encryption[i], HEX); 
         if(i%4 == 3)
           Serial.print(" ");
       }
       Serial.println();
       
       do_encryption(encryption);
       
       Serial.println("Crypto:");
       for(int i=0; i<8; ++i){
         Serial.print(encryption[i], HEX); 
         if(i%4 == 3)
           Serial.print(" ");
       }       
       Serial.println();
}
void test_print(){
	Serial.println("Plaintext:");
	sprintf(print_buf, "%02x%02x%02x%02x %02x%02x%02x%02x", encryption[0], encryption[1], encryption[2], encryption[3], encryption[4], encryption[5], encryption[6], encryption[7]);
	Serial.println(print_buf);

	do_encryption(encryption);

	Serial.println("Ciphertext:");
	sprintf(print_buf, "%02x%02x%02x%02x %02x%02x%02x%02x", encryption[0], encryption[1], encryption[2], encryption[3], encryption[4], encryption[5], encryption[6], encryption[7]);
	Serial.println(print_buf);
}

void loop() {
	while (Serial.available() > 0) {
		memset(encryption, 0x0, 8);
		Serial.readBytesUntil('\n', (char*) encryption, 8);
		//test_print();
                test2();
	}
}


