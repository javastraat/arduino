#include <PRESENTasm.h>
#include "PRESENTapi.h"

#define highByte(w) ((uint8_t) ((w) >> 8))
#define lowByte(w) ((uint8_t) ((w) & 0xff))

static unsigned char encryption[24], key[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
char print_buf[18];
void setup() {
	Serial.begin(9600);
	Serial.println("PRESENT-80 encrypter");
	memset(encryption, 0x0, 8);
	memcpy(encryption+8, key, 10);
        test_print();
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
		test_print();
	}
}


