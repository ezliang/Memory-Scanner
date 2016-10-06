#include <windows.h>

typedef enum _DATA_TYPE{ _BYTE, _SHORT, _DWORD, _FLOAT, _DOUBLE, _STRING, _INVALID } DATA_TYPE;

int GetScanType();
void ScanMenu(HANDLE proc);
int ChangeScanOpt();
DATA_TYPE GetValueAndSize(void*& val_loc, size_t& val_len);
int AskContScan();
void GetNewValue(void*& val_loc, const size_t  val_len, DATA_TYPE data_type);
void* WriteValueMemory(char* input, void*& val_loc, DATA_TYPE data_type, int val_len, int radix=10);
