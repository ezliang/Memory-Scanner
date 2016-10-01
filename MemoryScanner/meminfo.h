#include <Windows.h>

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
	void ScanMemory(unsigned long start, unsigned long stop, 
					 unsigned char* val, unsigned int len);
	HANDLE get_proc() const { return proc; }
	void PrintMemInfo() const;
private:
	HANDLE proc;
	MemoryBlockInfo* head;
	MemoryBlockInfo* last;
};