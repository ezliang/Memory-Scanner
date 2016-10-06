#include "menu.h"
#include "scanner.h"
#include <stdio.h>


int GetScanType(){

    int c;
    puts("1) Exact Scan");
    puts("2) Uninitialized Scan");
    puts("3) Exit");
    scanf_s(" %d", &c);
    return c;
}

void ScanMenu(HANDLE proc){
    Scanner s(proc);
    
#if _WIN64
    unsigned long long start = 0;
    unsigned long long end = 0x7fffffffffffffff;
#else
    unsigned long start = 0;
    unsigned long end = 0x7fffffff;
#endif
    size_t len = 0;
    void* val;
    unsigned long tmp;
    DATA_TYPE data_type;
    int c;

    c = ChangeScanOpt();
    while (c != 3) {
        switch (c) {
        case 1:
            puts("Enter address in hex");
            scanf_s(" %x", &tmp);
            if (tmp > end) {
                puts("Start address can't be greater than end");
            } else {
                start = tmp;
            }
            break;
        case 2:
            puts("Enter address in hex");
            scanf_s(" %x", &tmp);
            if (tmp < start) {
                puts("End address can't less than start");
            }
            else {
                end = tmp;
            }
            break;
        default:
            break;
        }
        c = ChangeScanOpt();
    }

    data_type = GetValueAndSize(val, len);

    if (data_type == _INVALID)
        return;

    s.InitScanMemory(start, end, val, len);
    s.PrintScanResults();
    free(val);

    c = AskContScan();
    int diff;
    while (c != 3) {
        switch (c) {
        case 1:
            GetNewValue(val, len, data_type);
            diff = s.get_num_results();
            s.ScanMemoryCont(val);
            s.PrintScanResults();
            printf("Reduced %d results\n", diff - s.get_num_results());
            free(val);
            break;
        case 2:

            break;
        default:
            break;
        }
        c = AskContScan();
    }

}

int ChangeScanOpt(){
    int c;
    puts("1) Change start address [Default: 0]");
    puts("2) Change end address [Default: 0]");
    puts("3) Start scan");

    scanf_s(" %d", &c);
    return c;
}

DATA_TYPE GetValueAndSize(void*& val_loc, size_t& val_len){
    
    int choice;
    //I've decided to use the largest data type to hold values then 
    //demote to the appropriate value size
    char input[16];
    int radix;

    puts("Select data type:");
    puts("\t1) Byte");
    puts("\t2) Short");
    puts("\t3) Dword");
    puts("\t4) Float");
    puts("\t5) Double");

    scanf_s(" %d", &choice);
    printf("Value: ");
    fflush(stdin);
    fgets(input, 15, stdin);
    
    char* c;
    if (c = strchr(input, '\n'))
        *c = '\x00';

    radix = 10;
    c = input;

    if (!strncmp(input, "0x", 2))
        radix = 16;

    //can cause the str conversions to fail but that seems like a user error
    switch (choice) {
    case 1:
        val_len = 1;
        val_loc = malloc(val_len);
        *(unsigned char*)val_loc = (unsigned char)strtoul(c, nullptr, radix);
        return _BYTE;
    case 2:
        val_len = sizeof(short);
        val_loc = malloc(val_len);
        *(unsigned short*)val_loc = (unsigned short)strtoul(c, nullptr, radix);
        return _SHORT;
    case 3:
        val_len = sizeof(unsigned long);
        val_loc = malloc(val_len);
        *(unsigned long*)val_loc = (unsigned long)strtoul(c, nullptr, radix);
        return _DWORD;
    case 4:
        val_len = sizeof(float);
        val_loc = malloc(val_len);
        *(float*)val_loc = (float)strtof(c, nullptr);
        return _FLOAT;
    case 5:
        val_len = sizeof(double);
        val_loc = malloc(val_len);
        *(double*)val_loc = (double)strtod(c, nullptr);
        return _DOUBLE;
    default:
        return _INVALID;
    }

}

int AskContScan(){
    int c;
    puts("1) Scan for value change");
    puts("2) Undo last scan");
    puts("3) End scan");
    scanf_s(" %d", &c);
    return c;
}

void GetNewValue(void*& val_loc, const size_t  val_len, DATA_TYPE data_type){

    char input[16];
    char* c = nullptr;
    printf("Value: ");
    fflush(stdin);
    fgets(input, 15, stdin);

    if (c = strchr(input, '\n'))
        *c = '\x00';

    if (!strncmp(input, "0x", 2))
        WriteValueMemory(input, val_loc, data_type, val_len, 16);
    else
        WriteValueMemory(input, val_loc, data_type, val_len);
}

void* WriteValueMemory(char* input, void*& val_loc, DATA_TYPE data_type, size_t val_len, int radix){

    switch (data_type) {
    case _BYTE:
        val_loc = malloc(val_len);
        *(unsigned char*)val_loc = (unsigned char)strtoul(input, nullptr, radix);
        break;
    case _SHORT:
        val_loc = malloc(val_len);
        *(unsigned short*)val_loc = (unsigned short)strtoul(input, nullptr, radix);
        break;
    case _DWORD:
        val_loc = malloc(val_len);
        *(unsigned long*)val_loc = (unsigned long)strtoul(input, nullptr, radix);
        break;
    case _FLOAT:
        val_loc = malloc(val_len);
        *(float*)val_loc = (float)strtof(input, nullptr);
        break;
    case _DOUBLE:
        val_loc = malloc(val_len);
        *(double*)val_loc = (double)strtod(input, nullptr);
        break;
    }
    return val_loc;
}