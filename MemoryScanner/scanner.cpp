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
bool ScanMemory(MemoryBlockList& mbl, unsigned long start, unsigned long stop, void* val, unsigned int len);
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

	unsigned int val = 0xdeadbeef;

	MemoryBlockList mbl(proc);

	ScanMemory(mbl, addr, mod_end, &val, sizeof(val));
	mbl.PrintMemInfo();
	CloseHandle(proc);
}


bool ScanMemory(MemoryBlockList& mbl, unsigned long start, unsigned long stop, void* val, unsigned int len){
	MEMORY_BASIC_INFORMATION mbi;

	for (unsigned long i = (unsigned long) start; i < (unsigned long) stop;){
		if (!VirtualQueryEx(mbl.get_proc(), (void*)i, &mbi, sizeof(MEMORY_BASIC_INFORMATION)))
			return false;
		mbl.AddNode(mbi);
		i += mbi.RegionSize;
	}
	return true;
}
