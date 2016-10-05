#include <windows.h>

typedef enum _DATA_TYPE{ _BYTE, _SHORT, _DWORD, _FLOAT, _DOUBLE, _STRING, _INVALID } DATA_TYPE;

int GetScanType();
void ScanMenu(HANDLE proc);
int ChangeScanOpt();
DATA_TYPE GetValueAndSize(void*& val_loc, size_t& val_len);
int AskContScan();
void GetNewValue(void*& val_loc, const size_t  val_len, DATA_TYPE dt);

