#include "meminfo.h"

void* MemoryBlockList::AddNode(MEMORY_BASIC_INFORMATION mbi){
	MemoryBlockInfo* new_block;

	

	new_block = new MemoryBlockInfo();
	new_block->next = nullptr;
	new_block->region_start = (void*)mbi.BaseAddress;
	new_block->region_size = mbi.RegionSize;


	return new_block;
}