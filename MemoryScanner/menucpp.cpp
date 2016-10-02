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
    unsigned char* val;
    unsigned long tmp;
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

    GetValueAndSize(val, len);

    if (len == INT_MAX)
        return;

    s.InitScanMemory(start, end, val, len);

    

    free(val);
}

int ChangeScanOpt(){
    int c;
    puts("1) Change start address [Default: 0]");
    puts("2) Change end address [Default: 0]");
    puts("3) Start scan");

    scanf_s(" %d", &c);
    return c;
}

void GetValueAndSize(unsigned char*& val_loc, size_t& val_len){
    
    int choice;
    //I've decided to use the largest data type to hold values then 
    //demote to the appropriate value size
    unsigned long long tmp;

    puts("Select data type:");
    puts("\t1) Byte");
    puts("\t2) Short");
    puts("\t3) Dword");
    puts("\t4) Float");
    puts("\t5) Double");

    scanf_s(" %d", &choice);
    printf("Value: ");

    switch (choice) {
    case 1:
        val_len = 1;
        val_loc = (unsigned char*)malloc(val_len);
        scanf_s("%u", &tmp);
        *val_loc = (unsigned char)tmp;
        break;
    case 2:
        val_len = sizeof(short);
        val_loc = (unsigned char*)malloc(val_len);
        scanf_s(" %hu", &tmp);
        *val_loc = (unsigned char)tmp;
        break;
    case 3:
        val_len = sizeof(unsigned long);
        val_loc = (unsigned char*)malloc(val_len);
        scanf_s(" %u", &tmp);
        *val_loc = (unsigned long)tmp;
        break;
    case 4:
        val_len = sizeof(float);
        val_loc = (unsigned char*)malloc(val_len);
        scanf_s(" %f", &tmp);
        *val_loc = (float)tmp;
        break;
    case 5:
        val_len = sizeof(double);
        val_loc = (unsigned char*)malloc(val_len);
        scanf_s(" %lf", &tmp);
        *val_loc = (double)tmp;
        break;
    default:
        val_len = INT_MAX;
    }


}