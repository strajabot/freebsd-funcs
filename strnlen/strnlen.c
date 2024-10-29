#include "stdint.h"
#include "stddef.h"

typedef union ptr {
	void* ptr;
	uint8_t* byte;
	uint32_t* word;
	uint64_t* dword;
	uint64_t value;
} ptr_t;

size_t strnlen_riscv(char *s_c, size_t maxlen) {
	ptr_t s = { .ptr = s_c };

	if(maxlen == 0) return 0;

	const uint64_t REP8_0X01 = 0x0101010101010101;
	const uint64_t REP8_0X80 = REP8_0X01 << 7;

	uint64_t end = s.value + maxlen + 7;
	ptr_t end_align = { .value = end & ~0b111 };
	
	ptr_t ptr = { .value = s.value & ~0b111 };

	uint64_t iter; 

	if(s.value != ptr.value) {
		
		// je l Ovo moze kao ((-s.value) << 3 ) & 0b11111 
		uint64_t off = (((-s.value) << 3) & 0b111111);
		uint64_t mask_start = REP8_0X01 >> off;

		iter = *ptr.dword;

		iter = iter | mask_start;

		uint64_t niter = ~iter;
		iter = iter - REP8_0X01;
		niter = niter & REP8_0X80;
		iter = iter & niter;

		// ptr is moved to next here, needs to be moved back in find_zero;
		ptr.dword = ptr.dword + 1;
		if(iter) {
			goto  find_zero;
		}

	}
	
	uint64_t len_unroll = (end_align.value - ptr.value);
	ptr_t end_unroll = { .value = ptr.value + (len_unroll & ~0b1111)};

	while(ptr.value != end_unroll.value) {
		iter = *ptr.dword;
		uint64_t iter2 =  *(ptr.dword+1);
		
		uint64_t niter = ~iter;
		uint64_t niter2 = ~iter2;
		iter = iter - REP8_0X01;
		iter2 = iter2 - REP8_0X01;
		niter = niter & REP8_0X80;
		niter2 = niter2 & REP8_0X80;
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

		uint64_t niter = ~iter;
		iter = iter - REP8_0X01;
		niter = niter & REP8_0X80;
		iter = iter & niter;
		

		// ptr is moved to next here, needs to be moved back in find_zero;
		ptr.dword = ptr.dword + 1;
		if(iter) {
			goto  find_zero;
		}


	}

	return maxlen;
find_zero:
	ptr.dword = ptr.dword - 1;
	iter = iter & (-iter);
	iter = iter >> 7;
	iter = iter * 0x0001020304050607ull;
	iter = iter >> 56;
	size_t zero_idx = ptr.value - s.value + iter;
	
	return maxlen ^ ((zero_idx ^ maxlen) & -(zero_idx < maxlen)); // min(zero_idx, maxlen);
}

