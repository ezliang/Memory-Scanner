#include "meminfo.h"

void* MemoryBlockList::AddNode(const MEMORY_BASIC_INFORMATION mbi){
	MemoryBlockInfo* new_block;

	new_block = new MemoryBlockInfo();
	
	if (!new_block)
		return nullptr;

	new_block->next = nullptr;
	new_block->region_start = (void*)mbi.BaseAddress;
	new_block->region_size = mbi.RegionSize;
	new_block->mem_block = new unsigned char[mbi.RegionSize];

	ReadProcessMemory(
		proc, 
		mbi.BaseAddress, 
		new_block->mem_block,
		mbi.RegionSize, 
		NULL);

	if (!head)
		head = new_block;

	if (!last){
		last = new_block;
	} else{
		last->next = new_block;
		last = new_block;
	}

	return new_block;
}

void MemoryBlockList::PrintMemInfo() const {
	MemoryBlockInfo* cur = head;


}