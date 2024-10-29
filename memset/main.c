#include <stdio.h>

extern void* memset_riscv(void* b, int c, unsigned long long size);

int main() {
	char b[64];
	int c = 0xDEADBEEF;
	memset_riscv(&b, c, 64);
	for(int i=0; i<64; i++) {
		printf("%.2x", b[i] & 0xFF);
	}
}
