#include "libFirmlaunch/firmlaunch.h"
#include "patches.h"

//ARM9
void patchSignatureChecks(u32* addr, u32 search_size){
	u8 sigpattern1[0x08] = {0x70, 0xB5, 0x22, 0x4D, 0x0C, 0x00, 0x69, 0x68};
	u8 sigpattern2[0x04] = {0xC0, 0x1C, 0x76, 0xE7};
	u8 sigpatch1[0x04] = {0x00, 0x20, 0x70, 0x47};
	u8 sigpatch2[0x04] = {0x00, 0x20, 0x76, 0xE7};
	
	patch(addr, search_size, sigpattern1, sigpatch1, sizeof(sigpattern1), sizeof(sigpatch1), 0);
	patch(addr, search_size, sigpattern2, sigpatch2, sizeof(sigpattern2), sizeof(sigpatch2), 0);
	return;
}

void restoreFirmlaunchhax(u32* addr, u32 search_size){
	u8 firmlaunchpattern[0x4] = {0x00, 0xFC, 0xFF, 0x01};
	u8 firmlaunchpatch[0x4] = {0x00, 0x00, 0x00, 0x24};
	
	patch(addr, search_size, firmlaunchpattern, firmlaunchpatch, sizeof(firmlaunchpattern), sizeof(firmlaunchpatch), 0);
	return;
}

void cleanITCM(){
	memset((void*)0x01FF8000, 0, 0x3700);
	memset((void*)0x01FFCDE4, 0, 0x21C);
	memset((void*)0x01FFF470, 0, 0xB90);
	return;
}

void patchARM9(u32* addr, u32 search_size){
	cleanITCM();
	patchSignatureChecks(addr, search_size);
	restoreFirmlaunchhax(addr, search_size);
	return;
}


//ARM11
void patchSvcAccess(u32* addr, u32 search_size){
	u8 svcpattern[0x04] = {0xEA, 0xFF, 0xFF, 0x0A};
	u8 svcpatch[0x04] = {0x00, 0xF0, 0x20, 0xE3};
	
	patch(addr, search_size, svcpattern, svcpatch, sizeof(svcpattern), sizeof(svcpatch), 0);
	return;
}

void patchARM11(u32* addr, u32 search_size){
	patchSvcAccess(addr, search_size);
	return;
}
