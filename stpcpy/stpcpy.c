#include "stdint.h"
#include <stdint.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef union ptr {
	void* ptr;
	uint8_t* byte;
	uint32_t* word;
	uint64_t* dword;
	uint64_t value;
} ptr_t;

#define PAGE_SIZE (1ull << 12)

char* stpcpy_8(char* restrict dstv, char* restrict srcv) {
	ptr_t dst = { .ptr = dstv };
	ptr_t src = { .ptr = srcv };
	
	uint64_t remain = 8 - (src.value & 0b111);
	
	uint64_t iter;
		
	uint64_t align4_off = (page_off) % 32 >> 3;

	switch (align4_off) {
		case 3:
		
		case 2:

		case 1:

		case 0:
	}

	return dstv;
}

char* stpcpy_riscv(char* restrict dstv, char* restrict srcv) {
	ptr_t dst = { .ptr = dstv };
	ptr_t src = { .ptr = srcv };

	uint64_t dst_align_rem = (-dst.value) & 0b111;

	uint64_t iter;

	if(dst_align_rem) {
		uint64_t src_align_off = src.value & 0b111;
		ptr_t src_align = { .value = src.value & ~0b111 };

		iter = *src_align.dword;
		
		uint8_t byte;
		
		uint8_t computed_jump = 8 - src_align_off;
		dst_align_rem = dst_align_rem - computed_jump;
		dst.value = dst.value + computed_jump;
		
		src.value += 8;
		
		iter = iter >> src_align_off;
		uint64_t iter_shift = iter >> 8;
		
		iter = iter >> 

		switch (computed_jump) {
			case 8:
				byte = (uint8_t) iter;
				*(dst.byte - 8) = (uint8_t) iter;
				if(!byte) goto ret_done;
				iter = iter >> 16;
			case 7:
				byte = (uint8_t) iter_shift;
				*(dst.byte - 7) = (uint8_t) iter_shift;
				if(!byte) goto ret_done;
				iter_shift = iter_shift >> 16;
			case 6:
				byte = (uint8_t) iter;
				*(dst.byte - 6) = (uint8_t) iter;
				if(!byte) goto ret_done;
				iter = iter >> 16;
			case 5:
				byte = (uint8_t) iter_shift;
				*(dst.byte - 5) = (uint8_t) iter_shift;
				if(!byte) goto ret_done;
				iter_shift = iter_shift >> 16;
			case 4:
				byte = (uint8_t) iter;
				*(dst.byte - 4) = (uint8_t) iter;
				if(!byte) goto ret_done;
				iter = iter >> 16;
			case 3:
				byte = (uint8_t) iter_shift;
				*(dst.byte - 3) = (uint8_t) iter_shift;
				if(!byte) goto ret_done;
				iter_shift = iter_shift >> 16;
			case 2:
				byte = (uint8_t) iter;
				*(dst.byte - 2) = (uint8_t) iter;
				if(!byte) goto ret_done;
				iter = iter >> 16;
			case 1:
				byte = (uint8_t) iter_shift;
				*(dst.byte - 1)= (uint8_t) iter_shift;
				if(!byte) goto ret_done;
				iter_shift = iter_shift >> 16;
		}

		
		if(dst_align_rem <= 0) goto dst_aligned;

		//src is aligned here
		iter = *src.dword;
		
		iter_shift = iter >> 8;

		computed_jump = dst_align_rem;
		dst.value = dst.value + dst_align_rem;

		switch (computed_jump) {
			case 7:
				byte = (uint8_t) iter_shift;
				*(dst.byte - 7) = (uint8_t) iter_shift;
				if(!byte) goto ret_done;
				iter_shift = iter_shift >> 16;
			case 6:
				byte = (uint8_t) iter;
				*(dst.byte - 6) = (uint8_t) iter;
				if(!byte) goto ret_done;
				iter = iter >> 16;
			case 5:
				byte = (uint8_t) iter_shift;
				*(dst.byte - 5) = (uint8_t) iter_shift;
				if(!byte) goto ret_done;
				iter_shift = iter_shift >> 16;
			case 4:
				byte = (uint8_t) iter;
				*(dst.byte - 4) = (uint8_t) iter;
				if(!byte) goto ret_done;
				iter = iter >> 16;
			case 3:
				byte = (uint8_t) iter_shift;
				*(dst.byte - 3) = (uint8_t) iter_shift;
				if(!byte) goto ret_done;
				iter_shift = iter_shift >> 16;
			case 2:
				byte = (uint8_t) iter;
				*(dst.byte - 2) = (uint8_t) iter;
				if(!byte) goto ret_done;
				iter = iter >> 16;
			case 1:
				byte = (uint8_t) iter_shift;
				*(dst.byte - 1)= (uint8_t) iter_shift;
				if(!byte) goto ret_done;
				iter_shift = iter_shift >> 16;
		}

		src.value += 8;
		
	}


dst_aligned:
	if(0 == 1) return dst.ptr; // expected expression bla bla

	//move backward if we overextended during fist load;
	src.value = dst.value - dst.value & 0b111;
	dst.value = dst.value & ~0b111;

	const uint64_t REP8_0X01 = 0x0101010101010101;
	const uint64_t REP8_0X80 = REP8_0X01 << 7;

	uint64_t diff = (dst.value - src.value) & 0b111;

	
	if(diff == 0) { 
		return stpcpy_8(dstv, srcv);
	}

	return dst.ptr;

ret_done:
	return dst.ptr;

}
