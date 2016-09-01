#include <string.h>

#include "arm11.h"

void setup(){
	__asm__ volatile (
		".WORD 0xF10C01C0 \n" //CPSID AIF ; Disable Interrupts
		"LDR SP, =0x27800000" //Set the Stack Pointer
	);
	
	while(1){
		ARM11Entry = 0;
		while(!ARM11Entry);
		((void (*)())ARM11Entry)();
	}
}

void ARM11(void* fptr){
	ARM11Entry = (u32)fptr;
	while(ARM11Entry);
}

void ARM11NoWait(void* fptr){
	ARM11Entry = (u32)fptr;
}

void ARM11Start(void){
	ARM11(setup);
}
