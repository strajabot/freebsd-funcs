#include "stdio.h"
#include "string.h"

// Function prototype for the SIMD memrchr_riscv implementation
void* memrchr_riscv(const void* buffer, int c, size_t size);

int main() {
    char big[256] __attribute__((aligned(16))) = { 0 };

    printf("found test\n");
    for(int align=0; align<24; align++) {
        char* buffer = big + align;
        for(int len=0; len<128; len++) {
            for(int i=0; i<len; i++) {
                memset(big, 1, 256);
                memset(&buffer[i+1], 0, len-(i+1));
                char* res = (char*) memrchr_riscv(buffer, 1, len);
                if(res != buffer + i) {
                    printf("len: %d, align: %d, expected: %p, found: %p\n", len, align, &buffer[i], res);
                }
            }
        }
    }

    printf("no_match test 1\n");
    for(int align=0; align<24; align++) {
        char* buffer = big + align;
        for(int len=0; len<128; len++) {
            memset(big, 1, 256);
            memset(&buffer[0], 0, len);
                char* res = (char*) memrchr_riscv(buffer, 1, len);
                if(res != NULL) {
                    printf("len: %d, align: %d, expected: %p, found: %p\n", len, align, NULL, res);
                }
        }
    }

    printf("no_match test 2\n");
    for(int align=0; align<24; align++) {
        char* buffer = big + align;
        for(int len=0; len<128; len++) {
            memset(big, 0, 256);
                char* res = (char*) memrchr_riscv(buffer, 1, len);
                if(res != NULL) {
                    printf("len: %d, align: %d, expected: %p, found: %p\n", len, align, NULL, res);
                }
        }
    }

    printf("found test - multiple 1s\n");
    for(int align=0; align<24; align++) {
        char* buffer = big + align;
        for(int len=0; len<128; len++) {
            for(int i=0; i<len; i++) {
                memset(big, 1, 256);
                memset(&buffer[i+1], 0, len-(i+1));
                if(i >=3)
                    buffer[i-2] = 1;
                char* res = (char*) memrchr_riscv(buffer, 1, len);
                if(res != buffer + i) {
                    printf("len: %d, align: %d, expected: %p, found: %p\n", len, align, &buffer[i], res);
                }
            }
        }
    }
    


    return 0;
}
