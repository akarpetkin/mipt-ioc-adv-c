#include <stdio.h>
#include <inttypes.h>

int main() {
    uint32_t n, k, max;
    scanf("%d %d", &n, &k);

    max = 0;
    for (uint8_t i = 32; i >= k; i--) {
        uint32_t lsh = n << (32-i);
        uint32_t rsh = lsh >> (32-k);

        if (rsh > max) {
            max = rsh;
        }
    }
  
    printf("%"PRIu32"\n", max);
    return 0;
}