#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <inttypes.h>

bool checkWeightLimit(uint32_t mass, const int* digits, int length) {
    uint32_t left_weight = mass;
    uint32_t right_weight = 0;
    
    for (int i = 0; i < length; i++) {
        uint32_t power = 1;
        for (int j = 0; j < i; j++) {
            power *= 3;
        }
        
        if (digits[i] == -1) {
            left_weight += power;
        } else if (digits[i] == 1) {
            right_weight += power;
        }
        
        if (left_weight > 1000000 || right_weight > 1000000) {
            return false;
        }
    }
    
    return true;
}

int main() {
    uint32_t mass;
    scanf("%lld", &mass);
    
    if (mass > 1000000) {
        printf("-1\n");
        return 0;
    }
    
    uint32_t n = mass;
    int digits[50] = {0};
    int length = 0;
    
    while (n > 0) {
        int remainder = n % 3;
        
        if (remainder == 2) {
            digits[length] = -1;
            n = n / 3 + 1;
        } else {
            digits[length] = remainder;
            n = n / 3;
        }
        length++;
    }
    
    if (!checkWeightLimit(mass, digits, length)) {
        printf("-1\n");
        return 0;
    }
    
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (digits[i] != 0) {
            count++;
        }
    }
    
    printf("%d\n", count);
    return 0;
}