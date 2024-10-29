#include <stdint.h>

typedef union ptr {
	void* ptr;
	uint8_t* byte;
	uint32_t* word;
	uint64_t* dword;
	uint64_t value;
} ptr_t;


char* strrchr_riscv(char* str, char c_in) {
    ptr_t ptr = { .ptr = str };

    uint8_t c = (uint8_t)c_in;

    const uint64_t REP8_0X01 = 0x0101010101010101;
    uint64_t REP8_0X80 = REP8_0X01 << 7;

    ptr_t save_align = { .value = 0 };
    uint64_t save_iter = ~0xff; 
    
    uint64_t cccccccc = c * REP8_0X01;

    uint64_t align_offset = (ptr.value & 0b111) << 3;

    ptr.value = ptr.value & ~0b111;

    REP8_0X80 = REP8_0X80 << align_offset;      // ignore bytes before start
    
    // register allocation
    uint64_t a1, a2, a3, a4;        
    do {
        a1 = *ptr.dword;                          // a1 -> data = *ptr
        a3 = ~a1;                               // a3 -> nhz = ~data
        a2 = a1 ^ cccccccc;                     // a2 -> iter = data^cccccccc
        a1 = a1 - REP8_0X01;                    // a1 -> hz = data - REP8_0X01
        a4 = ~a2;                               // a4 -> nhc = ~(iter^cccccccc)
        a1 = a1 & a3;                         // hz = hz & nhz
        a3 = a2 - REP8_0X01;                    // a3 -> hc = (iter^cccccccc) - REP8_0X01
        a1 = a1 & REP8_0X80;                    // hz = hz & REP8_0X80
        a3 = a3 & a4;                         // hc = hc & nhc
        a4 = a1 - 1;                            // a4 -> mask_end = hz - 1 
        a3 = a3 & REP8_0X80;                    // hc = hc & REP8_0X80
        a4 = a1 ^ a4;                         // mask_end = (mask_end) ^ hz
        ptr.value = ptr.value + 8;                  // ptr = ptr + 8 //setup for next iteration
        a3 = a3 & a4;                         // hc = hc & mask_end // prevent finding char past end
        REP8_0X80 = REP8_0X01 << 7;                 // reset mask for next iteration (no bytes should be ignored after first iteration)                
        a4 = ~a4;                               // mask_end = ~mask_end

        if(a3) {                                  // if (hc) found match ->  save 
            a2 = a2 | a4;                     // iter = iter | mask_end // mask zerobyte and past end
            save_align.value = ptr.value - 8;       // save_align = ptr - 8 // save iter of current iteration
            save_iter = a2;                       // save_iter = iter // save iter to later find the exact position
        }
    } while (!a1);                                // while(not_end)

lret:
    for (int i = 56; i >= 0; i -= 8) {
        if (((save_iter >> i) & 0xFF) == 0) {
            return (char*)save_align.byte + i / 8;
        }
    }

}