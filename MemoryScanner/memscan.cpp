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

	unsigned long pid = strtol(argv[1], NULL, 10);

	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	
	if (!proc) {
		fprintf(stderr,"Failed to open process\n");
        ExitShowError();
	}

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
	
	CloseHandle(proc);
}

