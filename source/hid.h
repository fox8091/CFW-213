#ifndef HID_H
#define HID_H

#include "types.h"

#define KEY_A		(1 << 0)
#define KEY_B		(1 << 1)
#define KEY_Select 	(1 << 2)
#define KEY_Start	(1 << 3)
#define KEY_Right	(1 << 4)
#define KEY_Left	(1 << 5)
#define KEY_Up		(1 << 6)
#define KEY_Down	(1 << 7)
#define KEY_R		(1 << 8)
#define KEY_L		(1 << 9)
#define KEY_X		(1 << 10)
#define KEY_Y		(1 << 11)

#define i2c_Power	(1 << 0)
#define i2c_Home	(1 << 2)
#define i2c_WiFi	(1 << 4)
#define i2c_Shut	(1 << 5)
#define i2c_Open	(1 << 6)

u32 HIDKeyStatus(void);
u8 HIDI2CStatus(void);

#endif
