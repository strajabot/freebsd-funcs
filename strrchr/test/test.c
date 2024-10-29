#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdalign.h>

extern char* strrchr_riscv(const char* str, int c);


void test_single(char* s, int align, size_t len, size_t x1, size_t x2) {
        
    /* without sentinels */
	s[len] = '\0';
    s[x1] = 'x';
    s[x2] = 'x';
	char* val = strrchr_riscv(s, 'x');
	if (val != s + x2) {
		fprintf(stderr, "align =  %d, len = %zu, x1 = %zu, x2 = %zu\n",
		    align, len, x1, x2);
		fprintf(stderr, "incorrect return value\n");
        exit(-1);
    }


	/* with '0' sentinels */
	s[-1] = '\0';
	s[len + 1] = '\0';
	val = strrchr_riscv(s, 'x');
	if (val != s + x2) {
		fprintf(stderr, "align =  %d, len = %zu, x1 = %zu, x2 = %zu\n",
		    align, len, x1, x2);
		fprintf(stderr, "incorrect return value\n");
        exit(-1);
    }

    /* with 'x' sentinels */
	s[-1] = 'x';
	s[len + 1] = 'x';
	val = strrchr_riscv(s, 'x');
	if (val != s + x2) {
	    fprintf(stderr, "align =  %d, len = %zu, x1 = %zu, x2 = %zu\n",
		    align, len, x1, x2);
		fprintf(stderr, "incorrect return value\n");
        exit(-1);
    }

	/* cleanup */
	s[-1] = '/';
    s[x1] = '/';
    s[x2] = '/';
    s[len] = '/';
}


int main() {
	char alignas(16) buffer[1 + 16 + 64 + 1 + 1];
	
    memset(buffer, '/', sizeof(buffer));
	
    for (int align = 1; align < 1 + 16; align++) {
		char *s = buffer + align;
		for (size_t len = 0; len <= 64; len++) {
            
            /* without sentinels */
	        s[len] = '\0';
	        
            char* val = strrchr_riscv(s, '\0');
	        if (val != s+len) {
	        	fprintf(stderr, "align =  %d, len = %zu\n",
	        	    align, len);
	        	fprintf(stderr, "incorrect return value\n");
                exit(-1);
            }

            val = strrchr_riscv(s, 'x');
	        if (val != NULL) {
	        	fprintf(stderr, "align =  %d, len = %zu\n",
	        	    align, len);
	        	fprintf(stderr, "incorrect return value\n");
                exit(-1);
            }

	        /* with '0' sentinels */
	        s[-1] = '\0';
	        s[len + 1] = '\0';
	        val = strrchr_riscv(s, 'x');
	        if (val != NULL) {
	        	fprintf(stderr, "align =  %d, len = %zu\n",
	        	    align, len);
	        	fprintf(stderr, "incorrect return value\n");
                exit(-1);
            }

            /* with 'x' sentinels */
	        s[-1] = 'x';
	        s[len + 1] = 'x';
	        val = strrchr_riscv(s, 'x');
	        if (val != NULL) {
	            fprintf(stderr, "align =  %d, len = %zu\n",
	        	    align, len);
	        	fprintf(stderr, "incorrect return value\n");
                exit(-1);
            }

	        /* cleanup sentinels*/
	        s[-1] = '/';
            s[len] = '/';

            // one match
            for(size_t x1=0; x1<len; x1++ ) {
                test_single(s, align, len, x1, x1);
            }

            // two matches
            for(size_t x1=0; x1<len; x1++ ) {
                for(size_t x2=x1; x2<len; x2++ ) {
                    test_single(s, align, len, x1, x1);
                }
            }
			
		}
	}
    
    return 0;
}