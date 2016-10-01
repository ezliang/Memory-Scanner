#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <winnt.h>
#include <stdlib.h>	
#include <stdio.h>
#include <iostream>
#include "error.h"
#include "meminfo.h"
#include "modinfo.h"

int main(int argc, char** argv){

	if (argc < 2){
		printf("Usage %s [pid]\n", argv[0]);
		return 0;
	} 

	unsigned long addr = 0;
	unsigned long mod_end;
	DWORD pid = strtol(argv[1], NULL, 10);
	DWORD mod_size = 0;
	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	
	if (!proc) {
		fprintf(stderr,"Failed to open process\n");
		return 0;
	}

	addr = GetBaseAddress(proc);

	if (!addr)
		ExitShowError();

	mod_size = GetModSize(proc,(HMODULE)addr);

	if (!mod_size)
		ExitShowError();

	printf("Mod size 0x%x\n", mod_size);
	mod_end = addr + mod_size;

	printf("Loaded at %p\n", addr);

	unsigned char val = 0x41;

	MemoryBlockList mbl(proc);

	mbl.InitScanMemory(addr, mod_end, (unsigned char*)&val, sizeof(val));
	CloseHandle(proc);
}
