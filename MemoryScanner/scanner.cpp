#include <stdio.h>
#include "scanner.h"
#include "error.h"
#include <algorithm>
#include <utility>

#define MakePtr( cast, ptr, addValue ) (cast)( (DWORD_PTR)(ptr) + (DWORD_PTR)(addValue))

Scanner::~Scanner() {

	MemoryBlockInfo* cur = head;
	MemoryBlockInfo* next;

	while (cur) {
		next = cur->next;
		delete cur->mem_block;
		delete cur;
		cur = next;
	}
  

}

void* Scanner::AddNode(const MEMORY_BASIC_INFORMATION mbi) {
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
HANDLE mutex;
void Scanner::InitScanMemory(unsigned long start, unsigned long stop,
                                     void* val, unsigned int len) {
	MEMORY_BASIC_INFORMATION mbi;

    scan_len = len;

    //generate a list of checkable memory addresses only writable addresses are interesting
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

    MemoryBlockInfo* cur = head;
    const size_t num_threads = 4;
    mutex = CreateMutex(NULL, false, NULL);

    ScanData sd[num_threads];

    HANDLE thread_handles[num_threads];

    while (cur) {
        for (size_t i = 0; i < num_threads && cur; ++i){
            sd[i].val = val;
            sd[i].results = &scan_locs;
            sd[i].scan_len = scan_len;
            sd[i].cur = cur;
            if (!(thread_handles[i] = CreateThread(0, NULL, CompareRegion, &sd[i], 0, NULL)))
                ExitShowError();
            cur = cur->next;
        }
        WaitForMultipleObjects(num_threads, thread_handles, false, INFINITE);
    }

    //to join the threads just sort the vector
    std::sort(scan_locs.begin(), scan_locs.end(), []
        (const std::pair<unsigned int,unsigned int> lhs, 
        const std::pair<unsigned int,unsigned int> rhs){
        return lhs.first < rhs.first;
    });

    CloseHandle(mutex);
    //Should I truncate the linked list for blocks that don't make it in the search?
    //Right now I probably shouldn't since repopulating the list is more overhead than 
    //worrying about scan iterations
}

void Scanner::ScanMemoryCont(void* new_val){
    MemoryBlockInfo* cur = head;
    std::vector<std::pair<unsigned long, unsigned long>>::iterator it;

    _ReScanMemory();

    for (it = scan_locs.begin(); it != scan_locs.end(); ) {
        //since the locations are pushed in order of mem_blocks this should work
        if ((unsigned long)cur->mem_block != it->first){
            while (cur && (unsigned long)cur->region_start != it->first) {
                cur = cur->next;
            }
        }

        if (!cur) break;

        //Keep only blocks that changed to the search value
        if (memcmp(MakePtr(void*, cur->mem_block, it->second),
            new_val, scan_len)){
            it = scan_locs.erase(it);
        } else {
            //block has change value;
            ++it;
        }
        
    }
}

void Scanner::EndScan(){

    MemoryBlockInfo* cur = head;
    MemoryBlockInfo* next;

    while (cur) {
        next = cur->next;
        delete cur->mem_block;
        delete cur;
        cur = next;
    }

    scan_locs.clear();
    scan_len = 0;
    head = nullptr;
    last = nullptr;
}

void Scanner::PrintMemInfo() const {
    MemoryBlockInfo* cur = head;

    while (cur) {
        printf("Address:\t0x%p\nSize:\t\t0x%x\n", cur->region_start, cur->region_size);
        cur = cur->next;
    }
}


static DWORD WINAPI CompareRegion(void* param){
    ScanData* sd = (ScanData*)param;
    MemoryBlockInfo* cur = sd->cur;
    std::pair <unsigned long, unsigned long> block_loc;
    unsigned long offset = 0;
    unsigned char* region_end;

    unsigned long wait_res;

    
    region_end = MakePtr(unsigned char*,cur->mem_block, cur->region_size);

    for (DWORD offset = 0; offset < cur->region_size; offset++) {

        if (!memcmp(MakePtr(void*, cur->mem_block, offset), sd->val, sd->scan_len)) {
            //Decided to save a pointer to the mem_block copy since it won't be freed 
            //until destructor is called or the val at the location isn't needed
            block_loc = std::make_pair(
                (unsigned long)cur->region_start,
                offset);
            wait_res = WaitForSingleObject(mutex, INFINITE);
            sd->results->push_back(block_loc);
            ReleaseMutex(mutex);
        }
    }
    
    return true;
}

//This shoudn't really be called externally I can only think of it being
//called when you need to check if memory is changed
void Scanner::_ReScanMemory(){

    MemoryBlockInfo* cur = head;
    while (cur) {

        ReadProcessMemory(proc, 
            cur->region_start,
            cur->mem_block,
            cur->region_size, 
            NULL);

        cur = cur->next;
    }

}

void Scanner::PrintScanResults() const{
    MemoryBlockInfo* cur = head;

    for (size_t i = 0; i < scan_locs.size(); ++i) {
        printf("%p\n", MakePtr(void*, scan_locs[i].first, scan_locs[i].second));
    }

}

void PrintMem(void* addr, size_t len, size_t block_len){
    void* tmp = (unsigned char*)addr;
    for (size_t i = 0; i < len; ) {
        
        switch (block_len) {
        
        case 1:
            printf("%1x\n", *(unsigned char*)tmp);
            break;
        case 2:
            printf("%2x\n", *(unsigned short*)tmp);
            break;
        case 4:
            printf("%x\n", *(unsigned int*)tmp);
            break;
        case 8:
            printf("%llx\n", *(unsigned long long*)tmp);
            break;
        }
       
        tmp = MakePtr(unsigned char*,tmp,block_len);
        i += block_len;
    }

}