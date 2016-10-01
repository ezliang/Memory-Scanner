#include <Windows.h>

struct MemoryBlockInfo{
	void* region_start;
	DWORD region_size;
	

	MemoryBlockInfo* next;
};

class MemoryBlockList{

public:
	MemoryBlockList() : head(nullptr), last(nullptr), cur(nullptr) {};
	void* AddNode(MEMORY_BASIC_INFORMATION mbi);

private:
	MemoryBlockInfo* head;
	MemoryBlockInfo* last;
	MemoryBlockInfo* cur;
};