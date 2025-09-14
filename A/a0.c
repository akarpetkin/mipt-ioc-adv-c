#include <stdio.h>

int main() {
    int n;
    scanf("%d", &n);
    
    int num, max, count = 0;
    
    for (int i = 0; i < n; i++) {
        scanf("%d", &num);
        
        if (i == 0 || num > max) {
            max = num;
            count = 1;
        } else if (num == max) {
            count++;
        }
    }
    
    printf("%d\n", count);
    return 0;
}