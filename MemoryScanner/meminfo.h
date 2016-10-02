#include <Windows.h>
#include <vector>


struct MemoryBlockInfo{
	void* region_start;
	DWORD region_size;
	BYTE* mem_block;
	MemoryBlockInfo* next;
};

class MemoryBlockList{

public:
	MemoryBlockList(HANDLE proc) : head(nullptr), last(nullptr), proc(proc) {};
	~MemoryBlockList();

	void* AddNode(const MEMORY_BASIC_INFORMATION mbi);
	void InitScanMemory(unsigned long start, unsigned long stop, 
                        unsigned char* val, unsigned int len);
    void ScanMemoryCont(unsigned char* new_val);
    void EndScan();

	HANDLE get_proc() const { return proc; }
	void PrintMemInfo() const;
private:
    void _ReScanMemory();
    HANDLE proc;
    MemoryBlockInfo* head;
    MemoryBlockInfo* last;
    unsigned int scan_len;
    unsigned char* scan_val;
    std::vector<std::pair<unsigned long, unsigned long>> scan_locs;
   
};

void PrintMem(void* addr, size_t len, size_t block_len=4);