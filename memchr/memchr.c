#include "stdint.h"
#include "stddef.h"
#include <stdint.h>

typedef union ptr {
	void* ptr;
	uint8_t* byte;
	uint32_t* word;
	uint64_t* dword;
	uint64_t value;
} ptr_t;

void* memchr_riscv(char* ba, int ca, size_t len) {
   
	if(len == 0) return 0;

	ptr_t b = { .ptr = ba };

	uint64_t c = (uint8_t) ca;

	uint64_t REP8_0x01 = 0x0101010101010101ull;
	uint64_t REP8_0x80 = 0x8080808080808080ull;
    
	uint64_t cccccccc = c * REP8_0x01;
    
	uint64_t end = b.value + len;
	end = (-(end > b.value)) & end; //in case of overflow sets end to 0, this causes the last read byte to be at max 0xFF...FF
	end = end + 7;

	ptr_t end_align = { .value = end & ~0b111 };

	uint64_t off = (((-b.value) << 3) & 0b111111);
	ptr_t ptr = { .value = b.value & ~0b111 };

	uint64_t iter;

	uint64_t mask_start = (-(uint64_t)(ptr.value < b.value)) >> off;

	iter = *ptr.dword;
        
	iter = iter ^ cccccccc;

	iter = iter | mask_start;

	uint64_t niter = ~iter;
	iter = iter - REP8_0x01;
	niter = niter & REP8_0x80;
	iter = iter & niter;

	// ptr is moved to next here, needs to be moved back in find_zero;
	ptr.dword = ptr.dword + 1;
	if(iter) {
		goto  find_zero;
	}
	
	uint64_t len_unroll = (end_align.value - ptr.value);
	ptr_t end_unroll = { .value = ptr.value + (len_unroll & ~0b1111)};

	while(ptr.value != end_unroll.value) {
		iter = *ptr.dword;
		uint64_t iter2 =  *(ptr.dword+1);
		
		iter = iter ^ cccccccc;
		iter2 = iter2 ^ cccccccc;

		uint64_t niter = ~iter;
		uint64_t niter2 = ~iter2;
		iter = iter - REP8_0x01;
		iter2 = iter2 - REP8_0x01;
		niter = niter & REP8_0x80;
		niter2 = niter2 & REP8_0x80;
		iter = iter & niter;
		iter2 = iter2 & niter2;

		// ptr is moved to next here, needs to be moved back in find_zero;
		ptr.dword = ptr.dword + 1;
		if(iter) {
			goto  find_zero;
		}

		iter = iter2;
	
		ptr.dword = ptr.dword + 1;
		if(iter2) {
			goto find_zero;
		}
		
	}

	if(ptr.value != end_align.value) { 
		
		iter = *ptr.dword;
		iter = iter ^ cccccccc;

		uint64_t niter = ~iter;
		iter = iter - REP8_0x01;
		niter = niter & REP8_0x80;
		iter = iter & niter;

		// ptr is moved to next here, needs to be moved back in find_zero;
		ptr.dword = ptr.dword + 1;
		if(iter) {
			goto  find_zero;
		}

	}
	
	return NULL;
find_zero:
	ptr.dword = ptr.dword - 1;
	iter = iter & (-iter);
	iter = iter >> 7;
	iter = iter * 0x0001020304050607ull;
	iter = iter >> 56;
	
	size_t zero_idx = ptr.value - b.value + iter;
	
	//if(zero_idx < len) {
	//    return (char*) (ptr.value + iter);
	//} else {
	//    return NULL;
	//}

	int64_t diff = -(int64_t)(zero_idx < len) ;	// slt, neg
	return (char*)((ptr.value + iter) & diff);   // and, ret

}



