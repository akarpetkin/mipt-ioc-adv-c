#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int count_three_digit_numbers(char *digits) {
    int len = strlen(digits);
    
    if (len < 3) {
        return 0;
    }
    
    int *unique_numbers = malloc(1000 * sizeof(int));
    int count = 0;
    
    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j < len; j++) {
            for (int k = j + 1; k < len; k++) {

                if (digits[i] == '0') continue;
                int num = (digits[i] - '0') * 100 + 
                         (digits[j] - '0') * 10 + 
                         (digits[k] - '0');
                
                int is_unique = 1;
                for (int m = 0; m < count; m++) {
                    if (unique_numbers[m] == num) {
                        is_unique = 0;
                        break;
                    }
                }
                
                if (is_unique) {
                    unique_numbers[count++] = num;
                }
            }
        }
    }
    
    free(unique_numbers);
    return count;
}

int main() {
    char str[255];
    scanf("%s", &str);
    
    int count = count_three_digit_numbers(str);
    
    printf("%d\n", count);
    return 0;
}