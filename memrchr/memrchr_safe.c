#include "stddef.h"
#include "stdint.h"

typedef union ptr {
  void *ptr;
  uint8_t *byte;
  uint32_t *word;
  uint64_t *dword;
  uint64_t value;
} ptr_t;

void *memrchr_riscv(void *b_void, int c_in, size_t len) {
  ptr_t b = { .ptr = b_void };

  if (len == 0)
    return NULL;

  uint8_t c = (uint8_t)c_in;

  const uint64_t REP8_0X01 = 0x0101010101010101;
  const uint64_t REP8_0X80 = REP8_0X01 << 7;

  uint64_t cccccccc = c * REP8_0X01;

  ptr_t start_align = {.value = b.value & ~0b111};
  
  
  ptr_t ptr;
  uint64_t iter;
  uint64_t hz, nhz;

  {
    uint64_t end = b.value + len;
    //handles ending bytes (shifting and xoring will cancel out a part of REP8_0x01
    ptr.value = end + 7;
    uint64_t mask_shift = -end;
    ptr.value = ptr.value & ~0b111;
    mask_shift = (mask_shift << 3) & 0b111111;
    iter = *(ptr.dword - 1);
    uint64_t rep_shift = REP8_0X01 >> mask_shift;
    iter = iter ^ cccccccc;
    uint64_t mask = REP8_0X01 ^ rep_shift;
    ptr.value = ptr.value - 8;
    iter = iter | mask;
  
    // has_zero
    nhz = ~iter;
    hz = iter - REP8_0X01;
    nhz = nhz & REP8_0X80;
    hz = hz & nhz;
    if (hz) {
      goto find_byte;
    }
  }
  
  if(ptr.value == start_align.value)
    return NULL;
  
  {
    // a branch to check if the length is a multiple of 16 would be unpredictable
    // accesses the eight byte twice if the lenght was multiple of 16
    uint64_t rem16 = ptr.value - start_align.value;
  
    iter = *(ptr.dword - 1);
    rem16 = rem16 & 0b1111;
    iter = iter ^ cccccccc;
    ptr.value = ptr.value - 8;
    // has_zero
    nhz = ~iter;
    hz = iter - REP8_0X01;
    nhz = nhz & REP8_0X80;
    rem16 = ptr.value - rem16;
    hz = hz & nhz;
    if (hz) {
      goto find_byte;
    }
    ptr.value = rem16 + 8;
  }
  
  while (ptr.value != start_align.value) {
    iter = *(ptr.dword - 1);
    uint64_t iter2 = *(ptr.dword -2);

    ptr.value = ptr.value - 8;
    
    iter = iter ^ cccccccc;
    iter2 = iter2 ^ cccccccc;

    // has_zero
    nhz = ~iter;
    uint64_t nhz2 = ~iter2;
    
    hz = iter - REP8_0X01;
    uint64_t hz2 = iter2 - REP8_0X01;
    
    nhz = nhz & REP8_0X80;
    nhz2 = nhz2 & REP8_0X80;
    
    hz = hz & nhz;
    hz2 = hz2 & nhz2;
    
    if (hz) {
      goto find_byte;
    }

    ptr.value = ptr.value - 8;
    iter = iter2;

    if (hz2) {
      goto find_byte;
    }

  }
  return NULL;

find_byte:
  for (int i = 56; i >= 0; i -= 8) {
    if (((iter >> i) & 0xFF) == 0) {
      if (ptr.byte + i / 8 >= b.byte)
        return ptr.byte + i / 8;
    }
  }
  return NULL;
}
