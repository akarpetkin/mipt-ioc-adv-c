#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define MAX_SIZE 100

struct pack_array {
    uint32_t array;    
    uint32_t count0 : 8;
    uint32_t count1 : 8; 
};

void array2struct(int input_array[], struct pack_array *result) {
    result->array = 0;
    result->count0 = 0;
    result->count1 = 0;
    
    for (int i = 0; i < 32; i++) {
        result->array |= (input_array[i] & 0x1) << (31 - i);
        
        if (input_array[i] == 0) {
            result->count0++;
        } else {
            result->count1++;
        }
    }
}

int main(void) {
    int test_array[MAX_SIZE];
    int num, count = 0;
    
    while (scanf("%d", &num) == 1 && count < MAX_SIZE) {
        if (num == 0 || num == 1) {
            test_array[count++] = num;
        }
    }
    
    struct pack_array result;
    
    array2struct(test_array, &result);

    printf("%"PRIu32" ""%"PRIu32" ""%"PRIu32"\n", result.array, result.count0, result.count1);
    
    return 0;
}
