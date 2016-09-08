#include "libFirmlaunch/firmlaunch.h"
#include "libFirmlaunch/aes.h"

#include "fatfs/ff.h"
#include "i2c.h"
#include "arm11.h"
#include "hid.h"
#include "patches.h"

#define CONFIG_SDMMC *((vu32 *)0x10000020)
#define SDMMC_MAGIC 0x340

typedef struct {
	u8 N3DSKey95[0x10];
	u8 N3DSKey96[0x10];
	u8 KeyX0x18[0x10];
	u8 KeyX0x1B[0x10];
	u8 KeyX0x25[0x10];
} KeyStorage;

FirmInfo initFirm(void){
	FirmInfo const temp = {((u32*)0x24000000), ((u32*)((u8*)temp.firm + temp.firm[0xA0/4])), 
				((u32)(temp.firm[0xA8/4])), ((u32*)((u8*)temp.firm + temp.firm[0x70/4])),
				((u32)(temp.firm[0x78/4]))};
	return temp;
}

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

KeyStorage initKeys(void){
	KeyStorage temp;

	FIL keyhandle;
	u32 keybr = 0;
	
	if(f_open(&keyhandle, "slot0x11key95.bin", FA_READ | FA_OPEN_EXISTING) == FR_OK){
		f_read(&keyhandle, temp.N3DSKey95, 0x10, &keybr);
		f_close(&keyhandle);
		set_normalKey(0x11, temp.N3DSKey95);
	}
	if(f_open(&keyhandle, "slot0x11key96.bin", FA_READ | FA_OPEN_EXISTING) == FR_OK){
		f_read(&keyhandle, temp.N3DSKey96, 0x10, &keybr);
		f_close(&keyhandle);
		set_normalKey(0x11, temp.N3DSKey96);
	}
	if(f_open(&keyhandle, "slot0x18keyX.bin", FA_READ | FA_OPEN_EXISTING) == FR_OK){
		f_read(&keyhandle, temp.KeyX0x18, 0x10, &keybr);
		f_close(&keyhandle);
		set_keyX(0x18, temp.KeyX0x18);
	}
	if(f_open(&keyhandle, "slot0x1BkeyX.bin", FA_READ | FA_OPEN_EXISTING) == FR_OK){
		f_read(&keyhandle, temp.KeyX0x1B, 0x10, &keybr);
		f_close(&keyhandle);
		set_keyX(0x1B, temp.KeyX0x1B);
	}
	if(f_open(&keyhandle, "slot0x25keyX.bin", FA_READ | FA_OPEN_EXISTING) == FR_OK){
		f_read(&keyhandle, temp.KeyX0x25, 0x10, &keybr);
		f_close(&keyhandle);
		set_keyX(0x25, temp.KeyX0x25);
	}
	
	return temp;
}

int main(void){
	CONFIG_SDMMC = SDMMC_MAGIC; //Undocumented CONFIG Register, Allows SD/MMC Access
	
	FATFS sdmc;
	f_mount(&sdmc, "0:", 0);
	
	if(HIDKeyStatus() & KEY_R){
		ARM11(main_loop);
	}
	
	FIL firmhandle;
	u32 firmbr = 0;
	FirmInfo firminfo = initFirm();
	KeyStorage keys = initKeys();
	if(f_open(&firmhandle, "firm.bin", FA_READ | FA_OPEN_EXISTING) == FR_OK){
		f_read(&firmhandle, firminfo.firm, f_size(&firmhandle), &firmbr);
		f_close(&firmhandle);
		
		firm_setup(firminfo, keys.N3DSKey95, keys.N3DSKey96);
		//Remind me to add firmprot
		patchARM11(firminfo.arm11bin, firminfo.arm11size);
		patchARM9(firminfo.arm9bin, firminfo.arm9size);
		firmlaunch(firminfo);
	}
	
	MCU_ShutDown(); //Don't return!
	while(1); //Safety spin!
}
