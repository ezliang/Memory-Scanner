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
    unsigned long start = 0;
    unsigned long end = 0x7fffffff;
    unsigned int len = 0;
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

    data_type = GetValueAndSize((void*)val, len);

    if (data_type == INVALID)
        return;

    s.InitScanMemory(start, end, val, len);
    s.PrintScanResults();
    free(val);

    c = AskContScan();
    while (c != 2) {
        switch (c) {
        case 1:
            GetNewValue((void*)val, len, data_type);
            s.ScanMemoryCont(val);
            s.PrintScanResults();
            free(val);
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
        val_loc = (unsigned char*)malloc(val_len);
        *(unsigned char*)val_loc = (unsigned char)strtoul(c, nullptr, radix);
        return BYTE;
    case 2:
        val_len = sizeof(short);
        val_loc = (unsigned char*)malloc(val_len);
        *(unsigned short*)val_loc = (unsigned short)strtoul(c, nullptr, radix);
        return SHORT;
    case 3:
        val_len = sizeof(unsigned long);
        val_loc = (unsigned char*)malloc(val_len);
        *(unsigned long*)val_loc = (unsigned long)strtoul(c, nullptr, radix);
        return DWORD;
    case 4:
        val_len = sizeof(float);
        val_loc = (unsigned char*)malloc(val_len);
        *(float*)val_loc = (float)strtof(c, nullptr);
        return FLOAT;
    case 5:
        val_len = sizeof(double);
        val_loc = (unsigned char*)malloc(val_len);
        *(double*)val_loc = (double)strtod(c, nullptr);
        return DOUBLE;
    default:
        return INVALID;
    }


}

int AskContScan(){
    int c;


}

void GetNewValue(void*& val_loc, const size_t  val_len, DATA_TYPE data_type){

    char input[16];
    int radix;

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

    switch (data_type) {
    case BYTE:
        val_loc = (unsigned char*)malloc(val_len);
        *(unsigned char*)val_loc = (unsigned char)strtoul(c, nullptr, radix);
    case SHORT:
        val_loc = (unsigned char*)malloc(val_len);
        *(unsigned short*)val_loc = (unsigned short)strtoul(c, nullptr, radix);
    case DWORD:
        val_loc = (unsigned char*)malloc(val_len);
        *(unsigned long*)val_loc = (unsigned long)strtoul(c, nullptr, radix);
    case FLOAT:
        val_loc = (unsigned char*)malloc(val_len);
        *(float*)val_loc = (float)strtof(c, nullptr);
    case DOUBLE:
        val_loc = (unsigned char*)malloc(val_len);
        *(double*)val_loc = (double)strtod(c, nullptr);
    }
    
    

}