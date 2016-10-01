#include <stdio.h>
#include "meminfo.h"

MemoryBlockList::~MemoryBlockList() {

	MemoryBlockInfo* cur = head;
	MemoryBlockInfo* next;

	while (cur) {
		next = cur->next;
		delete cur->mem_block;
		delete cur;
		cur = next;
	}

}

void* MemoryBlockList::AddNode(const MEMORY_BASIC_INFORMATION mbi) {
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

	if (!last) {
		last = new_block;
	} else {
		last->next = new_block;
		last = new_block;
	}

	return new_block;
}

void MemoryBlockList::ScanMemory(unsigned long start, unsigned long stop,
				 unsigned char* val, unsigned int len) {
	MEMORY_BASIC_INFORMATION mbi;

	for (unsigned long i = (unsigned long)start; i < (unsigned long)stop;){
		if (!VirtualQueryEx(proc, (void*)i, &mbi, sizeof(MEMORY_BASIC_INFORMATION)))
			break;
		this->AddNode(mbi);
		i += mbi.RegionSize;
	}

}


void MemoryBlockList::PrintMemInfo() const {
	MemoryBlockInfo* cur = head;

	while (cur){
		printf("Address:\t0x%p\nSize:\t\t0x%x\n",cur->region_start,cur->region_size);
		cur = cur->next;
	}
}

