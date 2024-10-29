#include <stddef.h>
#include <string.h>
#include <stdio.h>

extern int strlen_riscv(const char* str);

static char* test_str = "Hello world, Strahinja";

int main() {
	char str[256];
	
	int size = strlen(test_str);
	printf("Etalon: %d\n", size);

	for(int i=0; i<8; i++) {
		for(int j=size; j>=0; j--) {
			memset(str, 0, 256);
			strncpy(str+i, test_str, j);
			printf("%d, %d\n", (int)strlen(str+i), strlen_riscv(str+i));
		}
	}
	
	return 0;
}

