#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <winnt.h>
#include <stdlib.h>	
#include <stdio.h>
#include "error.h"

unsigned long GetBaseAddress(HANDLE proc);
DWORD GetModSize(HMODULE mod_base);

int main(int argc, char** argv){

	if (argc < 2){
		printf("Usage %s [pid]\n", argv[0]);
		return 0;
	} 

	unsigned long addr = 0;
	DWORD pid = strtol(argv[1], NULL, 10);
	DWORD mod_size = 0;
	MEMORY_BASIC_INFORMATION mbi;
	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	
	if (!proc) {
		fprintf(stderr,"Failed to open process\n");
		return 0;
	}

	addr = GetBaseAddress(proc);

	mod_size = GetModSize((HMODULE)addr);
	

	printf("Loaded at %p\n", addr);

	VirtualQueryEx(proc, (void*)addr, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
		
		printf("Base Address: 0x%p\nAllocation Base: %p AllocationProtect: %d\nRegion Size: %u\nState: %d\nProtect: %d\nType: %d\n\n", mbi.BaseAddress, mbi.AllocationBase, mbi.AllocationProtect, mbi.RegionSize, mbi.State, mbi.Protect, mbi.Type);
		addr += mbi.RegionSize;
	
	

	CloseHandle(proc);
}

unsigned long GetBaseAddress(HANDLE proc){
	HMODULE hmods[0x400];
	DWORD bytes_read;
	char proc_name[MAX_PATH];
	char* c;

	GetModuleFileNameExA(proc, NULL, proc_name, MAX_PATH);

	if (c = strrchr(proc_name, '\\')){
		strcpy_s(proc_name, c+1);
	}

	if (EnumProcessModulesEx(proc, hmods, sizeof(hmods), &bytes_read, LIST_MODULES_ALL)) {
		
		for (size_t i = 0; i < bytes_read / sizeof(HMODULE); ++i){

			char mod_name[MAX_PATH];
			if (GetModuleBaseNameA(proc, hmods[i], mod_name, sizeof(mod_name))){
				if (!strcmp(mod_name,proc_name)){
					return (unsigned long) hmods[i];
				}
			}
			else{
				ExitProcess();
			}
		}
	}

	
	return NULL;
}

DWORD GetModSize(HMODULE mod_base){
	

}