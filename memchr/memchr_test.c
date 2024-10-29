#include <stdint.h>
#include <stdio.h>
#include <string.h>

void* memchr_riscv(void*, int, unsigned long long);

int main() {

	char test[32] = {0};
	
	for(int i=0; i<32; i++) {
		test[i] = i;
	}
	
	for(int i=0; i<7; i++) {
		for(int j=i; j<32; j++) {
			void* addr = memchr_riscv(test+i, j, 32-i);
			printf("i=%d, j=%d, %llu: error %llu\n", i, j, (unsigned long long)addr, (unsigned long long)(test+j)-(unsigned long long)(addr));	
		}
	}
	
	printf("not found check\n");
	for(int i=0; i<7; i++) {
		for(int j=i; j<32; j++) {
			void* addr = memchr_riscv(test+i, 32, 32-i);
			printf("i=%d, j=%d, %llu: error %llu\n", i, j, (unsigned long long)addr, (unsigned long long)(addr));	
		}
	}

	printf("before start test\n");
	for(int i=0; i<7; i++) {
		for(int j=i; j<32; j++) {
			memset(test, j, i);
			void* addr = memchr_riscv(test+i, j, 32-i);
			printf("i=%d, j=%d, %llu: error %llu\n", i, j, (unsigned long long)addr, (unsigned long long)(test+j)-(unsigned long long)(addr));	
			for(int i=0; i<32; i++) {
				test[i] = i;
			}
		}
	}

	printf("SIZE_MAX test\n");
	for(int i=0; i<7; i++) {
		for(int j=i; j<32; j++) {
			memset(test, j, i);
			void* addr = memchr_riscv(test+i, j, SIZE_MAX);
			printf("i=%d, j=%d, %llu: error %llu\n", i, j, (unsigned long long)addr, (unsigned long long)(test+j)-(unsigned long long)(addr));	
			for(int i=0; i<32; i++) {
				test[i] = i;
			}
		}
	}
	
	printf("len <8 test\n");
	for(int i=0; i<7; i++) {
		memset(test, 0, 32);
		for(int j=i; j<i+3; j++) {
			test[j] = j+1;
		}
		void* addr = memchr_riscv(test+i, 0, 3);
		printf("i=%d, %llu: error %llu\n", i, (unsigned long long)addr, (unsigned long long)(addr));	
	}

	return 0;
}


