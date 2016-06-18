#pragma once

#include <string.h>
#include "types.h"

void patchARM11(u32* FIRM, u32 search_size);
void patchARM9(u32* FIRM, u32 search_size);