#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

typedef union {
    float f;
    uint32_t u;
} converter;

uint32_t extractExp(float num) {
    converter cvt;
    cvt.f = num;

    uint32_t exp_bits = (cvt.u >> 23) & 0xFF;
    return (cvt.u >> 23) & 0xFF;
}

int main(void) {
    float value;

    scanf("%f", &value);
    printf("%"PRIu32, extractExp(value));

    return 0;
}