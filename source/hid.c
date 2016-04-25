#include "hid.h"
#include "i2c.h"

u32 HIDKeyStatus(void){
	return ~*((vu32 *)0x10146000);
}

u8 HIDI2CStatus(void){
	return i2cReadRegister(I2C_DEV_MCU, 0x10);
}