#ifndef ARM11_H
#define ARM11_H

#include "types.h"

#define ARM11Entry *((vu32 *)0x1FFFFFF8)

void ARM11(void* fptr);
void ARM11Start(void);

#endif