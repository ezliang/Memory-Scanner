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



    s.InitScanMemory(start, end, val, len);

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



}