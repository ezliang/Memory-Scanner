#include <stdio.h>
#include "meminfo.h"
#include <utility>

#define MakePtr( cast, ptr, addValue ) (cast)( (DWORD_PTR)(ptr) + (DWORD_PTR)(addValue))

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

void MemoryBlockList::InitScanMemory(unsigned long start, unsigned long stop,
                                     unsigned char* val, unsigned int len) {
	MEMORY_BASIC_INFORMATION mbi;

    scan_val = new unsigned char[len];
    memcpy(scan_val, val, len);
    scan_len = len;

	for (unsigned long query_addr = (unsigned long)start; query_addr < (unsigned long)stop;){
		if (!VirtualQueryEx(proc, (void*)query_addr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)))
			break;
        if (mbi.State & MEM_COMMIT &&
            mbi.Protect & (PAGE_EXECUTE_READWRITE |
            PAGE_READWRITE |
            PAGE_EXECUTE_WRITECOPY) &&
            (mbi.Protect & PAGE_GUARD) != PAGE_GUARD) {
            this->AddNode(mbi);
        }
		query_addr += mbi.RegionSize;
	}

    MemoryBlockInfo* cur;
    DWORD offset = 0;
    unsigned char* region_end;
    std::pair <unsigned long, unsigned long> block_loc;
    cur = head;

    while (cur) {
        region_end = (unsigned char*)((DWORD_PTR)cur->mem_block + (DWORD_PTR)cur->region_size);
        
        for (DWORD offset = 0; offset < cur->region_size; offset++) {
            
            if (!memcmp(MakePtr(void*, cur->mem_block, offset), val, len)) {

                //Decided to save a pointer to the mem_block copy since it won't be freed 
                //until destructor is called or the val at the location isn't needed
                block_loc = std::make_pair((unsigned long)cur->mem_block, 
                            offset);
                scan_locs.push_back(block_loc);
                printf("Found: %p\n", MakePtr(void*, cur->region_start , offset));
            }
        }
        cur = cur->next;
    }

}


void MemoryBlockList::PrintMemInfo() const {
    MemoryBlockInfo* cur = head;

    while (cur) {
        printf("Address:\t0x%p\nSize:\t\t0x%x\n",cur->region_start,cur->region_size);
        cur = cur->next;
	}
}
