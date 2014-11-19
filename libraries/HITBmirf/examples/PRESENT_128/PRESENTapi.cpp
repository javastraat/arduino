/*
 *  c file for implementing easy to use crypto
 */
#include <stdint.h>
#include <PRESENTapi.h>

#define highByte(w) ((uint8_t) ((w) >> 8))
#define lowByte(w) ((uint8_t) ((w) & 0xff))

extern "C" void encrypt();
extern "C" void decrypt();

void do_encryption(unsigned char* encryption){
  	// http://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_reg_usage
	asm("push r29");
	asm("push r28");
	asm("push r17");
	asm("push r16");
	asm("push r15");
	asm("push r14");
	asm("push r13");
	asm("push r12");
	asm("push r11");
	asm("push r10");
	asm("push r9");
	asm("push r8");
	asm("push r7");
	asm("push r6");
	asm("push r5");
	asm("push r4");
	asm("push r3");
	asm("push r2");
	asm("mov r26, %0":: "l" (lowByte((unsigned int)encryption)));
	asm("mov r27, %0":: "l" (highByte((unsigned int)encryption)));
	encrypt();
	asm("clr r1");
	asm("pop r2");
	asm("pop r3");
	asm("pop r4");
	asm("pop r5");
	asm("pop r6");
	asm("pop r7");
	asm("pop r8");
	asm("pop r9");
	asm("pop r10");
	asm("pop r11");
	asm("pop r12");
	asm("pop r13");
	asm("pop r14");
	asm("pop r15");
	asm("pop r16");
	asm("pop r17");
	asm("pop r28");
	asm("pop r29");
}

void do_decryption(unsigned char* encryption){
  	// http://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_reg_usage
	asm("push r29");
	asm("push r28");
	asm("push r17");
	asm("push r16");
	asm("push r15");
	asm("push r14");
	asm("push r13");
	asm("push r12");
	asm("push r11");
	asm("push r10");
	asm("push r9");
	asm("push r8");
	asm("push r7");
	asm("push r6");
	asm("push r5");
	asm("push r4");
	asm("push r3");
	asm("push r2");
	asm("mov r26, %0":: "l" (lowByte((unsigned int)encryption)));
	asm("mov r27, %0":: "l" (highByte((unsigned int)encryption)));
	decrypt();
	asm("clr r1");
	asm("pop r2");
	asm("pop r3");
	asm("pop r4");
	asm("pop r5");
	asm("pop r6");
	asm("pop r7");
	asm("pop r8");
	asm("pop r9");
	asm("pop r10");
	asm("pop r11");
	asm("pop r12");
	asm("pop r13");
	asm("pop r14");
	asm("pop r15");
	asm("pop r16");
	asm("pop r17");
	asm("pop r28");
	asm("pop r29");
}
