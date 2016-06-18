#include "libFirmlaunch/firmlaunch.h"
#include "libFirmlaunch/aes.h"

#include "fatfs/ff.h"
#include "i2c.h"
#include "arm11.h"
#include "hid.h"
#include "patches.h"

#define FIRM ((u32*)0x24000000)

#define arm9bin ((u32*)((void*)FIRM + FIRM[0xA0/4]))
#define arm9size ((u32)(FIRM[0xA8/4]))

#define arm11bin ((u32*)((void*)FIRM + FIRM[0x70/4]))
#define arm11size ((u32)(FIRM[0x78/4]))

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

int main(void){
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
		
		firm_setup(FIRM, N3DSKey95, N3DSKey96);
		/* FIRM Partition Update (Credit to Delebile) */
		if (!(*((vu32 *)0x101401C0) & 0x3)){ //Check for a9lh (Credit to AuroraWright)
			u8 FIRMUpdate[] = { 0x00, 0x28, 0x01, 0xDA, 0x04, 0x00 };
			for (u32 i = 0; i < arm9size; i+=2){
				if (memcmp((void*)(arm9bin+i), "exe:/%016llx/.firm", 0x12) == 0){
					for (i -= 0x100; i < FIRM[0xA8/4]; i+=2){
						if (memcmp((void*)(arm9bin+i), &FIRMUpdate, 6) == 0){
							*((vu16 *)(void*)(arm9bin+i)) = 0x2000;
							*((vu16 *)(void*)(arm9bin+i+2)) = 0x46C0;
							break;
						}
					}
					break;
				}
			}
		}
		patchARM11(arm11bin, arm11size);
		patchARM9(arm9bin, arm9size);
		firmlaunch(FIRM);
	}
	
	MCU_ShutDown(); //Don't return!
}
