#include "stdint.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

union ptr_t {
	void* ptr;
	uint8_t* byte;
	uint32_t* word;
	uint64_t* dword;
	uint64_t value;
};

void* memcpy_8(void* dstv, void* srcv, uint64_t len) {
	union ptr_t dst;
	union ptr_t src;
	
	dst.ptr = dstv;
	src.ptr = srcv;

	uint64_t min = MIN((-dst.value) & 0b111, len);
	len -= min;

	while(min) {
		min--;
		*dst.byte++ = *src.byte++;
	}

	if(len == 0) return dstv;

	union ptr_t end_align;

	end_align.value = (dst.value + len) & ~0b111;
	len = len & 0b111;

	while(dst.value != end_align.value) {
		*dst.dword++ = *src.dword++;
	}
	
	while(len != 0) {
		len--;
		*dst.byte++ = *src.byte++;
	}
	
	return dstv;
}

void* memcpy_4(void* dstv, void* srcv, uint64_t len) {
	union ptr_t dst;
	union ptr_t src;
	
	dst.ptr = dstv;
	src.ptr = srcv;

	
	uint64_t min = MIN((-dst.value) & 0b11, len);
	len -= min;

	while(min) {
		min--;
		*dst.byte++ = *src.byte++;
	}

	if(len == 0) return dstv;

	union ptr_t end_align;

	end_align.value = (dst.value + len) & ~0b11;
	len = len & 0b11;

	while(dst.value != end_align.value) {
		*dst.word++ = *src.word++;
	}
	
	while(len != 0) {
		len--;
		*dst.byte++ = *src.byte++;
	}
	
	return dstv;
}

void* memcpy_riscv(void* dstv, void* srcv, uint64_t len) {
	
	if(!len) return dstv;

	uint64_t diff = (dstv - srcv) & 0b111;

	if(diff == 0) {
		return memcpy_8(dstv, srcv, len);	
	}
	
	if(diff == 4) {
		return memcpy_4(dstv, srcv, len);
	}

	union ptr_t dst;
	union ptr_t src;
	
	dst.ptr = dstv;
	src.ptr = srcv;
	
	uint64_t min = MIN((-dst.value) & 0b111, len);
	len -= min;

	while(min) {
		min--;
		*dst.byte++ = *src.byte++;
	}

	if(!len) return dstv;

	uint64_t right_shift = (src.value & 0b111) << 3;
	uint64_t left_shift = 64 - right_shift;

	uint64_t whole = (len >> 3);
	len = len & 0b111;
	
	union ptr_t iter;
	iter.value = src.value & ~0b111;
	src.value += (whole << 3);
	
	uint64_t low = *iter.dword++;
	low >>= right_shift;
	
	while(whole) {
		whole--;
		uint64_t high = *iter.dword++;
		*dst.dword++ = (high << left_shift) | low;
		low = high >> right_shift;
		
	}

	while(len--) {
		*dst.byte++ = *src.byte++;
	}

	return dstv;

}




