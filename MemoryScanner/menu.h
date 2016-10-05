#include <windows.h>

typedef enum _DATA_TYPE{ BYTE } DATA_TYPE;

int GetScanType();
void ScanMenu(HANDLE proc);
int ChangeScanOpt();
DATA_TYPE GetValueAndSize(void*& val_loc, size_t& val_len);