#include "modinfo.h"
#include <Psapi.h>

//Get's the executable's base address which is also a HMODULE
DWORD GetBaseAddress(HANDLE proc){
	HMODULE hmods[0x400];
	DWORD bytes_read;
	char proc_name[MAX_PATH];
	char* c;

	GetModuleFileNameExA(proc, NULL, proc_name, MAX_PATH);

	if (c = strrchr(proc_name, '\\')) {
		strcpy_s(proc_name, c + 1);
	}

	if (EnumProcessModulesEx(proc, hmods, sizeof(hmods), &bytes_read, LIST_MODULES_ALL)) {

		for (size_t i = 0; i < bytes_read / sizeof(HMODULE); ++i){

			char mod_name[MAX_PATH];
			if (GetModuleBaseNameA(proc, hmods[i], mod_name, sizeof(mod_name))) {
				if (!strcmp(mod_name, proc_name)){
					return (unsigned long)hmods[i];
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

	if (!ReadProcessMemory(proc, (LPCVOID)((DWORD_PTR) mod_base + (DWORD_PTR) dos_head.e_lfanew), &nt_head, sizeof(IMAGE_NT_HEADERS), NULL))
		return NULL;

	if (nt_head.Signature != IMAGE_NT_SIGNATURE)
		return NULL;

	return nt_head.OptionalHeader.SizeOfImage;

}