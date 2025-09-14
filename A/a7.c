#include <stdio.h>
#include <inttypes.h>

int main() {
    uint32_t n;
    scanf("%d", &n);
    
    uint32_t i = 0, val = 0;
    while (i < n) {
        uint32_t k;
        
        scanf("%d", &k);
        val = val^k;
        i++;
    }
    
    printf("%"PRIu32"\n", val);
    return 0;
}