#include "libFirmlaunch\firmlaunch.h"
#include "patches.h"

void patchARM9(u32* FIRM, u32 search_size){
	cleanITCM();
	patchSignatureChecks(FIRM, search_size);
	restoreFirmlaunchhax(FIRM, search_size);
	return;
}

void patchARM11(u32* FIRM, u32 search_size){
	patchSvcAccess(FIRM, search_size);
	return;
}

//Thanks to Aurora/TuxSH for this piece of code
u8* getProcess9(u8 *pos, u32 size, u32 *process9Size, u32 *process9MemAddr){
    u8 *off = memsearch(pos, "ess9", size, 4);

    *process9Size = *(u32 *)(off - 0x60) * 0x200;
    *process9MemAddr = *(u32 *)(off + 0xC);

    //Process9 code offset (start of NCCH + ExeFS offset + ExeFS header size)
    return off - 0x204 + (*(u32 *)(off - 0x64) * 0x200) + 0x200;
}


//ARM9
void patchSignatureChecks(u32* FIRM, u32 search_size){
	u8 sigpattern1[0x08] = {0x70, 0xB5, 0x22, 0x4D, 0x0C, 0x00, 0x69, 0x68};
	u8 sigpattern2[0x04] = {0xC0, 0x1C, 0x76, 0xE7};
	u8 sigpatch1[0x04] = {0x00, 0x20, 0x70, 0x47};
	u8 sigpatch2[0x04] = {0x00, 0x20, 0x76, 0xE7};
	
	patch(FIRM, search_size, sigpattern1, sigpatch1, sizeof(sigpattern1), sizeof(sigpatch1), 0);
	patch(FIRM, search_size, sigpattern2, sigpatch2, sizeof(sigpattern2), sizeof(sigpatch2), 0);
}

void restoreFirmlaunchhax(u32* FIRM, u32 search_size){
	u8 firmlaunchpattern[0x4] = {0x00, 0xFC, 0xFF, 0x01};
	u8 firmlaunchpatch[0x4] = {0x00, 0x00, 0x00, 0x24};
	
	patch(FIRM, search_size, firmlaunchpattern, firmlaunchpatch, sizeof(firmlaunchpattern), sizeof(firmlaunchpatch), 0);
}

void cleanITCM(){
	memset((void*)0x01FF8000, 0, 0x3700);
	memset((void*)0x01FFCDE4, 0, 0x21C);
	memset((void*)0x01FFF470, 0, 0xB90);
}


//ARM11
patchSvcAccess(u32* FIRM, u32 search_size){
	u8 svcpattern[0x04] = {0xEA, 0xFF, 0xFF, 0x0A};
	u8 svcpatch[0x0C] = {0x00, 0xF0, 0x20, 0xE3, 0x00, 0x00, 0x5A, 0xE3, 0x00, 0xF0, 0x20, 0xE3};
	
		patch(FIRM, search_size, svcpattern, svcpatch, sizeof(svcpattern), sizeof(svcpatch), 0);

}
