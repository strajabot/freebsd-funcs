#include <stddef.h>
#include <string.h>
#include <stdio.h>

extern void* strchrnul_riscv(const char* str, int c);

char str[64];
int main() {

	for(int i=0; i<8; i++) {
		for(int len=0; len<32; len++) {
			memset(str, 0, 64);
			for(int k=0; k<len; k++) {
				str[i+k] = k+1;  
			}
			for(int k=0; k<=len; k++) {
				void* addr = strchrnul_riscv(str+i, k+1);
				void* actual = strchrnul(str+i,k+1);
				if(addr != actual) {
					printf("%llu: error %lld\n", (unsigned long long)addr, (unsigned long long)actual - (unsigned long long)addr);	
				}
			}
		}
	}
	char *heh = "";
	void* addr_theirs = strchrnul(heh, 0);
	void* addr_mine = strchrnul_riscv(heh, 0);
	printf("%llu: error %lld\n", (unsigned long long)addr_mine, (unsigned long long)addr_theirs - (unsigned long long)addr_mine);	

	
	return 0;
}

