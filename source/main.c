#include "../libFirmlaunch/firmlaunch.h"
#include "../libFirmlaunch/aes.h"

#include "fatfs/ff.h"
#include "i2c.h"
#include "arm11.h"
#include "hid.h"

#define FIRM (u32*)0x24000000

void MCU_ShutDown(void){
	i2cWriteRegister(I2C_DEV_MCU, 0x20, 1);
	while(1);
}

void MCU_Reboot(void){
	i2cWriteRegister(I2C_DEV_MCU, 0x20, 4);
	while(1);
}

void main_loop(void){
	while(1){
		if(HIDKeyStatus() & KEY_B) break;
		if(HIDI2CStatus() & i2c_Power) MCU_ShutDown();
		if(HIDI2CStatus() & i2c_Home) MCU_Reboot();
	}
}

void _start(void){
	*((vu32 *)0x10000020) = 0x340; //Undocumented CONFIG Register, Allows SD/MMC Access
	
	FATFS sdmc;
	f_mount(&sdmc, "0:", 0);
	
	if(HIDKeyStatus() & KEY_R){
		ARM11(main_loop);
	}
	
	FIL handle;
	u32 br = 0;
	u8 N3DSKey95[0x10] = { 0x00 };
	u8 N3DSKey96[0x10] = { 0x00 };
	u8 KeyX0x25[0x10] = { 0x00 };
	if(f_open(&handle, "slot0x11key95.bin", FA_READ | FA_OPEN_EXISTING) == FR_OK){
		f_read(&handle, N3DSKey95, 0x10, &br);
		f_close(&handle);
	}
	if(f_open(&handle, "slot0x11key96.bin", FA_READ | FA_OPEN_EXISTING) == FR_OK){
		f_read(&handle, N3DSKey96, 0x10, &br);
		f_close(&handle);
	}
	if(f_open(&handle, "slot0x25keyX.bin", FA_READ | FA_OPEN_EXISTING) == FR_OK){
		f_read(&handle, KeyX0x25, 0x10, &br);
		f_close(&handle);
		set_keyX(0x25, KeyX0x25);
	}
	if(f_open(&handle, "firm.bin", FA_READ | FA_OPEN_EXISTING) == FR_OK){
		f_read(&handle, FIRM, f_size(&handle), &br);
		f_close(&handle);
		
		u8 sigpattern1[0x8] = {0x70, 0xB5, 0x22, 0x4D, 0x0C, 0x00, 0x69, 0x68};
		u8 sigpattern2[0x4] = {0xC0, 0x1C, 0x76, 0xE7};
		u8 sigpatch1[0x4] = {0x00, 0x20, 0x70, 0x47};
		u8 sigpatch2[0x4] = {0x00, 0x20, 0x76, 0xE7};
		
		firm_setup(FIRM, N3DSKey95, N3DSKey96);
		patch(((void*)FIRM + FIRM[0xA0/4]), FIRM[0xA8/4], sigpattern1, sigpatch1, sizeof(sigpattern1), sizeof(sigpatch1));
		patch(((void*)FIRM + FIRM[0xA0/4]), FIRM[0xA8/4], sigpattern2, sigpatch2, sizeof(sigpattern2), sizeof(sigpatch2));
		firmlaunch(FIRM);
	}
	
	MCU_ShutDown(); //Don't return!
}
