#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <winnt.h>
#include <stdlib.h>	
#include <stdio.h>
#include <iostream>
#include "error.h"
#include "scanner.h"
#include "modinfo.h"
#include "menu.h"

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
        ExitShowError();
	}

	addr = GetBaseAddress(proc);

	if (!addr)
		ExitShowError();

	mod_size = GetModSize(proc,(HMODULE)addr);

	if (!mod_size)
		ExitShowError();

    int c = GetScanType();
   
    while (c != 3) {
       
        switch (c) {
        case 1:
            ScanMenu(proc);
            break;
        case 2:
            break;
        default:
            break;
        }
        c = GetScanType();
    }

	//printf("Mod size 0x%x\n", mod_size);
	//mod_end = addr + mod_size;

	//printf("Loaded at %p\n", addr);

	//unsigned long val = 0xdeadbeef;
	
	CloseHandle(proc);
}

