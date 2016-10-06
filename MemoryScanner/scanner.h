#include <Windows.h>
#include <vector>


struct MemoryBlockInfo{
    void* region_start;

#if _WIN64
    unsigned long long region_size;
#else
    unsigned long region_size;
#endif
    unsigned char* mem_block;
    MemoryBlockInfo* next;
};

struct ScanData{
    void* val;
    unsigned int scan_len;
    MemoryBlockInfo* cur;
#if _WIN64
    std::vector<std::pair<unsigned long long, unsigned long long>>* results;
#else
    std::vector<std::pair<unsigned long, unsigned long>>* results;
#endif
};

//Scanner class knows nothing about data types only location and length
class Scanner{

public:
	Scanner(HANDLE proc) : head(nullptr), last(nullptr), proc(proc) {};
	~Scanner();

	void* AddNode(const MEMORY_BASIC_INFORMATION mbi);
	
#if _WIN64
    void InitScanMemory(unsigned long long start, unsigned long long stop,
                        void* val, size_t len);
#else
    void InitScanMemory(unsigned long start, unsigned long stop,
        void* val, unsigned int len);
#endif
    void ScanMemoryCont(void* new_val);
    void EndScan();
	void PrintMemInfo() const;
    void PrintScanResults() const;

#if _WIN64
    std::vector<std::pair<unsigned long long, unsigned long long>> scan_locs;
#else
    std::vector<std::pair<unsigned long, unsigned long>> scan_locs;
#endif
private:
    void _ReScanMemory();
    HANDLE proc;
    MemoryBlockInfo* head;
    MemoryBlockInfo* last;
    size_t scan_len;
};

void PrintMem(void* addr, size_t len, size_t block_len=4);
static DWORD WINAPI CompareRegion(void* param);