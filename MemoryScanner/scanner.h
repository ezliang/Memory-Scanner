#include <Windows.h>
#include <vector>


struct MemoryBlockInfo{
    void* region_start;
#if _WIN32
    unsigned long region_size;
#elif _WIN64
    unsigned long long region_size;
#endif
    unsigned char* mem_block;
    MemoryBlockInfo* next;
};

struct ScanData{
    void* val;
    unsigned int scan_len;
    MemoryBlockInfo* cur;
#if _WIN32
    std::vector<std::pair<unsigned long, unsigned long>>* results;
#elif _WIN64
    std::vector<std::pair<unsigned long long, unsigned long long>>* results;
#endif
};

//Scanner class knows nothing about data types only location and length
class Scanner{

public:
	Scanner(HANDLE proc) : head(nullptr), last(nullptr), proc(proc) {};
	~Scanner();

	void* AddNode(const MEMORY_BASIC_INFORMATION mbi);
#if _WIN32
	void InitScanMemory(unsigned long start, unsigned long stop, 
                        void* val, unsigned int len);
#elif _WIN64
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
#if _WIN32
    std::vector<std::pair<unsigned long, unsigned long>> scan_locs;
#else
    std::vector<std::pair<unsigned long long, unsigned long long>> scan_locs;
#endif
};

void PrintMem(void* addr, size_t len, size_t block_len=4);
static DWORD WINAPI CompareRegion(void* param);