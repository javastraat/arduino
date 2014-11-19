#include <Colorduino.h>
#include "Vector3.h"

#define DIV 1
#define DSQ (DIV + 1) * (DIV + 1)
#define PPD (32.0 / DIV)

Colorduino matrix = Colorduino(31,63,55);

ISR(TIMER2_OVF_vect) {
	cli();
	TCNT2 = 0x64;
	matrix.run();
	sei();
}

bool corners[8][3] = {
	{0,0,0}, {1,0,0},
	{0,1,0}, {1,1,0},
	{0,0,1}, {1,0,1},
	{0,1,1}, {1,1,1},
};

Vector3 vectors[DSQ * 2] = {};

void regenerate() {
	for(uint8_t i = DSQ; i < DSQ * 2; i++) {
		vectors[i].x = random(2049) / 1024.0 - 1;
		vectors[i].y = random(2049) / 1024.0 - 1;
		vectors[i].z = random(2049) / 1024.0 - 1;
	}
}

void shift() {
	for(uint8_t i = 0; i < DSQ; i++) {
		vectors[i].x = vectors[i + DSQ].x;
		vectors[i].y = vectors[i + DSQ].y;
		vectors[i].z = vectors[i + DSQ].z;
	}
	
	regenerate();
}

void setup() {
	Serial.begin(9600);
	delay(1000);
	Serial.print(DIV);
	Serial.print("\t");
	Serial.print(DSQ);
	Serial.print("\t");
	Serial.print(PPD);
	Serial.print("\n");
	
	matrix.setup();
	regenerate();
	shift();
}

Vector3 current = {0, 0, 0};
void loop() {
	for(uint8_t row = 0; row < 32; row++) {
		for(uint8_t col = 0; col < 32; col++) {
			current.x = (float)row / PPD;
			current.y = (float)col / PPD;
			
			Vector3 eased = current.ease();
			
			float sum = 0;
			for(uint8_t i = 0; i < 8; i++) {
				Vector3 displacement = current.sub(corners[i]);
				sum += displacement.dot(vectors[i]) * eased.influence(corners[i]);
			}
			
			sum = (sum * 2.5 + 1) * 256 / 2;
			
			unsigned char final;
			if(sum < 0) final = 0;
			else if(sum > 255) final = 255;
			else final = (unsigned char)sum;
			
			matrix.setPixel(col,row, final);
		}
	}
	
	delay(1);
	current.z += .1;
	while(current.z >= 1) {
		current.z -= 1;
		shift();
	};
}
