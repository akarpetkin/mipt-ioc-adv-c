#include <stdio.h>

char shift(char val, int n, char startSymbol) {
    int m = val - startSymbol;
    int p = (m + n) % 26;
    if (p < 0) p += 26;
}

int main() {
    int n;
    scanf("%d", &n);
    
    char c;
    while (1) {
        scanf("%c", &c);
        
        if (c == '.') {
            printf(".");
            break;
        }

        if (c == ' ') {
            printf(" ");
            continue;
        }
       
        // Обработка строчных букв
        if (c >= 'a' && c <= 'z') {
            printf("%c", 'a' + shift(c, n, 'a'));
        }
        // Обработка заглавных букв
        else if (c >= 'A' && c <= 'Z') {
            printf("%c", 'A' + shift(c, n, 'A'));
        }
    }
    
    return 0;
}