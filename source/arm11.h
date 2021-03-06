#pragma once

#include "types.h"

#define ARM11Entry *((vu32 *)0x1FFFFFF8)

void ARM11(void (*fptr)());
void ARM11NoWait(void (*fptr)());
void ARM11Start(void);
