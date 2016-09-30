#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <winnt.h>
#include <stdlib.h>	
#include <stdio.h>
#include <iostream>
#include "error.h"

#define MakePtr( cast, ptr, addValue ) (cast)( (DWORD_PTR)(ptr) + (DWORD_PTR)(addValue))

unsigned long GetBaseAddress(HANDLE proc);
DWORD GetModSize(HANDLE proc, HMODULE mod_base);

int main(int argc, char** argv){

	if (argc < 2){
		printf("Usage %s [pid]\n", argv[0]);
		return 0;
	} 

	unsigned long addr = 0;
	unsigned long mod_end;
	DWORD pid = strtol(argv[1], NULL, 10);
	DWORD mod_size = 0;
	MEMORY_BASIC_INFORMATION mbi;
	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	
	if (!proc) {
		fprintf(stderr,"Failed to open process\n");
		return 0;
	}

	addr = GetBaseAddress(proc);

	mod_size = GetModSize(proc,(HMODULE)addr);
	printf("Mod size 0x%x\n", mod_size);
	mod_end = addr + mod_size;

	printf("Loaded at %p\n", addr);

	for (unsigned long i = addr; i < mod_end;){
		VirtualQueryEx(proc, (void*)i, &mbi, sizeof(MEMORY_BASIC_INFORMATION));

		printf("Base Address: 0x%p\nAllocation Base: %p AllocationProtect: %d\nRegion Size: %u\nState: %d\nProtect: %d\nType: %d\n\n", mbi.BaseAddress, mbi.AllocationBase, mbi.AllocationProtect, mbi.RegionSize, mbi.State, mbi.Protect, mbi.Type);
		i += mbi.RegionSize;
	}
	

	CloseHandle(proc);
}

unsigned long GetBaseAddress(HANDLE proc){
	HMODULE hmods[0x400];
	DWORD bytes_read;
	char proc_name[MAX_PATH];
	char* c;

	GetModuleFileNameExA(proc, NULL, proc_name, MAX_PATH);

	if (c = strrchr(proc_name, '\\')) {
		strcpy_s(proc_name, c+1);
	}

	if (EnumProcessModulesEx(proc, hmods, sizeof(hmods), &bytes_read, LIST_MODULES_ALL)) {
		
		for (size_t i = 0; i < bytes_read / sizeof(HMODULE); ++i){

			char mod_name[MAX_PATH];
			if (GetModuleBaseNameA(proc, hmods[i], mod_name, sizeof(mod_name))) {
				if (!strcmp(mod_name,proc_name)){
					return (unsigned long) hmods[i];
				}
			}
		}
	}

	
	return NULL;
}

DWORD GetModSize(HANDLE proc, HMODULE mod_base){
	IMAGE_DOS_HEADER dos_head;
	IMAGE_NT_HEADERS nt_head;

	if (!mod_base)
		return NULL;

	if (!ReadProcessMemory(proc, mod_base, &dos_head, sizeof(IMAGE_DOS_HEADER), NULL))
		return NULL;
	
	if (dos_head.e_magic != IMAGE_DOS_SIGNATURE)
		return NULL;

	if (!ReadProcessMemory(proc, MakePtr(LPCVOID, mod_base, dos_head.e_lfanew), &nt_head, sizeof(IMAGE_NT_HEADERS), NULL))
		return NULL;
	
	if (nt_head.Signature != IMAGE_NT_SIGNATURE)
		return NULL;
	
	return nt_head.OptionalHeader.SizeOfImage;

}