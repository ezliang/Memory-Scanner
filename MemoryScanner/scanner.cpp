#include <Windows.h>
#include <Psapi.h>
#include <stdlib.h>	
#include <stdio.h>

int main(int argc, char** argv){

	if (argc < 2){
		printf("Usage %s [pid]\n", argv[0]);
		return 0;
	} 

	DWORD pid = strtol(argv[1], NULL, 10);
	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	unsigned long addr = 0;
	MEMORY_BASIC_INFORMATION mbi;

	if (!proc) {
		fprintf(stderr,"Failed to open process\n");
		return 0;
	}

	while (VirtualQueryEx(proc, (void*)addr, &mbi, sizeof(MEMORY_BASIC_INFORMATION))) {
		
		printf("%p %p %d %u %d %d %d\n", mbi.BaseAddress, mbi.AllocationBase, mbi.AllocationProtect, mbi.RegionSize, mbi.State, mbi.Protect, mbi.Type);
		addr += mbi.RegionSize;
	
	}
}