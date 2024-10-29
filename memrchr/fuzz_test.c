#include "stdio.h"
#include "string.h"
#include "stdlib.h"

extern void* memrchr(const void* buffer, int c, size_t size);
extern void* memrchr_riscv(void* buffer, int c, size_t size);

int main() {
    char big[256 + 24 + 1] __attribute__((aligned(16)));
    big[256+24] = 0;
    for(size_t attempt=0; attempt < 1000000000; attempt++) { 
        int align = random() % 24;
        char* buffer = big + align;
        int len = random() % 256;
        for(int i=0; i< 256+24; i++) {
            big[i] = random();
        }
        int c  = random();
        void* verified = memrchr(buffer, c, len);
        void* custom = memrchr_riscv(buffer, c, len);

        if(verified != custom) {
            printf("FAILED\n"); 
            printf("%.s\n",big);
            printf("align=%d, len=%d, c=%d\n", align, len, c);
            return -1;
        }   
    }


    return 0;
}
