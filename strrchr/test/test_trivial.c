#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

extern char* strrchr_riscv(const char* str, int c);

void test_strrchr_simd() {
    // Test case 1: Character occurs multiple times
    const char *test_string_1 = "abracadabra";
    char target_1 = 'a';
    char *result_1 = strrchr(test_string_1, target_1);
    assert(result_1 == test_string_1 + 10);  // Last 'a' is at position 10
    printf("Test 1 Passed: '%s' found last 'a' at position %ld\n", test_string_1, result_1 - test_string_1);

    // Test case 2: Character occurs at the beginning
    const char *test_string_2 = "apple";
    char target_2 = 'p';
    char *result_2 = strrchr_riscv(test_string_2, target_2);
    printf("%zu %zu\n", (size_t)result_2, (size_t)(test_string_2+2));
    assert(result_2 == test_string_2 + 2);  // Last 'p' is at position 3
    printf("Test 2 Passed: '%s' found last 'p' at position %ld\n", test_string_2, result_2 - test_string_2);

    // Test case 3: Character occurs at the end
    const char *test_string_3 = "banana";
    char target_3 = 'n';
    char *result_3 = strrchr_riscv(test_string_3, target_3);
    assert(result_3 == test_string_3 + 4);  // Last 'n' is at position 5
    printf("Test 3 Passed: '%s' found last 'n' at position %ld\n", test_string_3, result_3 - test_string_3);

    // Test case 4: Character does not exist in the string
    const char *test_string_4 = "grape";
    char target_4 = 'z';
    char *result_4 = strrchr_riscv(test_string_4, target_4);
    assert(result_4 == NULL);  // 'z' is not in "grape"
    printf("Test 4 Passed: '%s' found no occurrence of 'z'\n", test_string_4);

    // Test case 6: Character occurs at various random positions in a large string
    srand(time(NULL));  // Seed for randomness
    char random_string[1000];
    for (int i = 0; i < 999; i++) {
        random_string[i] = (rand() % 26) + 'a';  // Random lowercase letter
        if(random_string[i] == 'x') random_string[i]--;
    }
    random_string[999] = '\0';  // Null-terminate the string

    // Place the character 'x' randomly
    int pos = rand() % 999;
    random_string[pos] = 'x';

    char *result_6 = strrchr_riscv(random_string, 'x');
    assert(result_6 == random_string + pos);  // 'x' should be at the random position
    printf("Test 6 Passed: Random string found 'x' at position %d\n", pos);
}

int main() {
    // Run tests for strrchr function with SIMD optimizations
    test_strrchr_simd();
    return 0;
}