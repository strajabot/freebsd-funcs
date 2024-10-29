#include "stdint.h"
#include "stdio.h"

#include "string.h"

extern void* memcpy_riscv(void* dst, const void* src, uint64_t len);

int main() {
	
	uint8_t src[256];
	uint8_t dst[256];

	for(int i=1; i<=256; i++) {
		src[i] = i;
	}

	for(int src_off=0; src_off<16; src_off++) {
		for(int dst_off=0; dst_off<16; dst_off++) {
			for(int len=0; len<128; len++) {
				memset(dst, 0, 256);
				memcpy_riscv(dst+dst_off, src+src_off, len);

				for(int i=0; i<dst_off; i++) {
					if(dst[i] != 0) 
						printf("src_off=%d, dst_off=%d, len=%d, i=%d, BEFORE_START: %d\n", src_off, dst_off, len, i, dst[i]);
				}
				for(int i=0; i<len; i++) {
					if(dst[i+dst_off] != src[i+src_off]) 
						printf("src_off=%d, dst_off=%d, len=%d, i=%d, WRONG_VAL: %d\n", src_off, dst_off, len, i, dst[i+dst_off]);
				}
				for(int i=dst_off+len; i<256; i++) {
					if(dst[i] != 0) 
						printf("src_off=%d, dst_off=%d, len=%d, i=%d, AFTER_END: %d\n", src_off, dst_off, len, i, dst[i]);
				}
			}


		}
	}



	return 0;
}


