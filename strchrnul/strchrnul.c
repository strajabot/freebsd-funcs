#include "stdint.h"

static inline void* find_char(char* ptr, uint64_t iter) {
    const uint64_t FIND_ZERO = 0x0001020304050607;
    iter = iter & -iter;
    iter >>= 7;

    iter = (iter * FIND_ZERO) >> 56;
    return ptr + iter;

}

void* strchrnul_riscv(const char *cs, int c32) {

    uint64_t c = (uint64_t)((uint8_t) c32);

    uint64_t REP8_0x01 = 0x0101010101010101ull;
    uint64_t REP8_0x80 = 0x8080808080808080ull;

    uint64_t cccccccc = c * REP8_0x01;

    uint64_t align_offset = (uint64_t)cs & 0b111;
    uint64_t* s =  (uint64_t*)((uint64_t)cs & ~0b111);

    uint64_t iter;

    if(align_offset) {
        iter = *s;
        uint64_t mask_start = (REP8_0x01 >> (64- align_offset*8));
    
        uint64_t has_zero = iter | mask_start;
        has_zero = (has_zero - REP8_0x01) & ~has_zero & REP8_0x80;
        
        iter ^= cccccccc;
        iter |= mask_start;

        iter = (iter - REP8_0x01) & ~iter & REP8_0x80;
        iter |= has_zero;
        if(iter != 0)
            return find_char((char*)s, iter);
        
        s += 1;
    }

    uint64_t has_zero;

    while(1) {
        iter = *s;
        
        has_zero = iter;
        has_zero = (has_zero - REP8_0x01) & ~has_zero & REP8_0x80;
        has_zero = has_zero & -has_zero;
        
        iter ^= cccccccc;
        iter = (iter - REP8_0x01) & ~iter & REP8_0x80;
        iter |= has_zero;
        if(iter != 0)
            return find_char((char*)s, iter);
        
        s += 1;
    }
    
}
