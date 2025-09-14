#include <stdio.h>
#include <inttypes.h>

uint32_t ror(uint32_t n, uint32_t k) {
  k = k % (sizeof(n)*8);
  return (n >> k) | (n << (sizeof(n)*8 - k));
}

int main() {
    uint32_t n, k;
    scanf("%d %d", &n, &k);
  
    printf("%"PRIu32"\n", ror(n, k));
    return 0;
}