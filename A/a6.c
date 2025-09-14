#include <stdio.h>
#include <inttypes.h>

int main() {
    uint32_t n;
    scanf("%d", &n);

    for (uint8_t i = 0; i < 8; i++) {
        n = n ^ (1 << 31-i);
    }
  
    printf("%"PRIu32"\n", n);
    return 0;
}