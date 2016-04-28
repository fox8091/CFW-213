#include "libFirmlaunch\firmlaunch.h"
#include "patches.h"

void patchARM9(u32* FIRM, u32 search_size){
	u8 sigpattern1[0x8] = {0x70, 0xB5, 0x22, 0x4D, 0x0C, 0x00, 0x69, 0x68};
	u8 sigpattern2[0x4] = {0xC0, 0x1C, 0x76, 0xE7};
	u8 sigpatch1[0x4] = {0x00, 0x20, 0x70, 0x47};
	u8 sigpatch2[0x4] = {0x00, 0x20, 0x76, 0xE7};
	
	patch(FIRM, search_size, sigpattern1, sigpatch1, sizeof(sigpattern1), sizeof(sigpatch1));
	patch(FIRM, search_size, sigpattern2, sigpatch2, sizeof(sigpattern2), sizeof(sigpatch2));
	return;
}

void patchARM11(u32* FIRM, u32 search_size){
	u8 svcpattern[0x4] = {0xEA, 0xFF, 0xFF, 0x0A};
	u8 svcpatch[0x4] = {0x00, 0xF0, 0x20, 0xE3};
	
	patch(FIRM, search_size, svcpattern, svcpatch, sizeof(svcpattern), sizeof(svcpatch));
	return;
}