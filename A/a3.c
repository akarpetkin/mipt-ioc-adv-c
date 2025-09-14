#include <stdio.h>
#include <inttypes.h>

int main() {
    uint32_t n, k;
    scanf("%d %d", &n, &k);

    uint32_t lsh = n << (32-k);
    uint32_t rsh = lsh >> (32-k);
  
    printf("%"PRIu32"\n", rsh);
    return 0;
}