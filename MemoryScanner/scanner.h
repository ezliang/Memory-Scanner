#include <Windows.h>
#include <vector>
#include "config.h"

struct MemoryBlockInfo{
    void* region_start;
#if ARCH == 32
    unsigned long region_size;
#elif ARCH == 64 
    unsigned long long region_size;
#endif
    unsigned char* mem_block;
    MemoryBlockInfo* next;
};

struct ScanData{
    void* val;
    unsigned int scan_len;
    MemoryBlockInfo* cur;
#if ARCH == 32
    std::vector<std::pair<unsigned long, unsigned long>>* results;
#elif ARCH == 64
    std::vector<std::pair<unsigned long long, unsigned long long>>* results;
#endif
};

//Scanner class knows nothing about data types only location and length
class Scanner{

public:
	Scanner(HANDLE proc) : head(nullptr), last(nullptr), proc(proc) {};
	~Scanner();

	void* AddNode(const MEMORY_BASIC_INFORMATION mbi);
#if ARCH == 32
	void InitScanMemory(unsigned long start, unsigned long stop, 
                        void* val, unsigned int len);
#elif ARCH == 64
    void InitScanMemory(unsigned long long start, unsigned long long stop,
        void* val, unsigned int len);
#endif
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
#if ARCH == 32
    std::vector<std::pair<unsigned long, unsigned long>> scan_locs;
#else
    std::vector<std::pair<unsigned long long, unsigned long long>> scan_locs;
#endif
};

void PrintMem(void* addr, size_t len, size_t block_len=4);
static DWORD WINAPI CompareRegion(void* param);