#include <stdio.h>
#include <inttypes.h>

int main() {
    uint32_t n, bits;
    scanf("%d", &n);

    bits = 0;
    for (uint8_t i = 0; i < 32; i++) {
        uint32_t bit = (n & ((uint32_t)1 << i)) >> i;

        if (bit > 0) {
           bits++;
        }
    }
  
    printf("%"PRIu32"\n", bits);
    return 0;
}