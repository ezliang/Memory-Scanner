#include <Windows.h>
DWORD GetBaseAddress(HANDLE proc);
DWORD GetModSize(HANDLE proc, HMODULE mod_base);