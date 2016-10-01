#include <Windows.h>

struct MemoryBlockInfo{
	void* region_start;
	DWORD region_size;
	DWORD prot;

	MemoryBlockInfo* next;
};

class MemoryBlockList{

public:
	MemoryBlockList() : head(nullptr), last(nullptr), cur(nullptr) {};
	void* AddNode();

private:
	MemoryBlockInfo* head;
	MemoryBlockInfo* last;
	MemoryBlockInfo* cur;
};