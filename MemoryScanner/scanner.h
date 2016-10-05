#include <Windows.h>
#include <vector>


struct MemoryBlockInfo{
	void* region_start;
	unsigned long region_size;
	unsigned char* mem_block;
	MemoryBlockInfo* next;
};
//Scanner class knows nothing about data types only location and length
class Scanner{

public:
	Scanner(HANDLE proc) : head(nullptr), last(nullptr), proc(proc) {};
	~Scanner();

	void* AddNode(const MEMORY_BASIC_INFORMATION mbi);
	void InitScanMemory(unsigned long start, unsigned long stop, 
                        void* val, unsigned int len);
    void ScanMemoryCont(void* new_val);
    void EndScan();
	void PrintMemInfo() const;
    void PrintScanResults() const;
private:
    void _ReScanMemory();
    HANDLE proc;
    MemoryBlockInfo* head;
    MemoryBlockInfo* last;
    unsigned int scan_len;
    std::vector<std::pair<unsigned long, unsigned long>> scan_locs;
   
};

void PrintMem(void* addr, size_t len, size_t block_len=4);