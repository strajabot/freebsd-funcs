#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include <stdint.h>

extern char* stpcpy_riscv(char* restrict dstv, char* restrict srcv);

char rand_char() {
    return rand() % ('z' - 'a' + 1) + 'a';
}

int main() {
    
    char srcb[256] __attribute__((aligned(8)));
    char dstb1[256] __attribute__((aligned(8)));
    char dstb2[256] __attribute__((aligned(8)));
    
    for(int i = 0; i<256; i++) {
        srcb[i] = rand_char();
        dstb1[i] = '0';
        dstb2[i] = '0';
    }

    for(int srcoff = 0; srcoff < 8; srcoff++) {
        char* src = &srcb[srcoff + 8];
        for(int dstoff = 0; dstoff < 8; dstoff++) {
            char* dst1 = &dstb1[dstoff + 8];
            char* dst2 = &dstb2[dstoff + 8];
            for (int len=0; len<128; len++) {
                src[len] = '\0';
                char* str = stpcpy_riscv(dst1, src);
                char* str_expected = stpcpy(dst2, src);
                uint64_t len1 = (uint64_t)str - (uint64_t)dst1;
                uint64_t len2 = (uint64_t)str_expected - (uint64_t)dst2; 
                if(len1 != len2) {
                    printf("error at: srcoff=%d, dstoff=%d, len=%d\n", srcoff, dstoff, len);
                    printf("expected return value: %p, got %p\n", str_expected, str);
                    return -1;
                }

                for(int i=0; i<len; i++ ) {
                    if(dst1[i] != dst2[i]) {
                        printf("error at: srcoff=%d, dstoff=%d, len=%d\n", srcoff, dstoff, len);
                        printf("expected value: %d, got %d, at index %d\n", dst2[i], dst1[i], i);
                        return -1;
                    }
                }
                src[len] = rand_char();
            }

        }
    }

    printf("passed align dst test");
    return 0;
}