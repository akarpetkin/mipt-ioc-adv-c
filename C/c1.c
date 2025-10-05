#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1024

void compute_z_function(char *s, int *z, int n) {
    memset(z, 0, n * sizeof(int));

    for (int i = 1; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (s[j] == s[j - i]) {
                z[i]++;
            } else {
                break;
            }
        }
    }
}

int max_suffix_prefix(char *str1, char *str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    
    // str2 + '#' + str1
    int total_len = len1 + len2 + 1;
    char *combined = (char *)malloc(total_len + 1);
    strcpy(combined, str2);
    combined[len2] = '#';
    strcpy(combined + len2 + 1, str1);
    combined[total_len] = '\0';
    
    int *z = (int *)malloc(total_len * sizeof(int));
    compute_z_function(combined, z, total_len);

    int max_len = 0;
    for (int i = len2 + 1; i < total_len; i++) {

        if (z[i] > 0 && i + z[i] == total_len) {
            max_len = z[i];
            break;
        }
    }
    
    free(combined);
    free(z);
    return max_len;
}

int max_prefix_suffix(const char *str1, const char *str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    
    // str1 + '#' + str2
    int total_len = len1 + len2 + 1;
    char *combined = (char *)malloc(total_len + 1);
    strcpy(combined, str1);
    combined[len1] = '#';
    strcpy(combined + len1 + 1, str2);
    combined[total_len] = '\0';
    
    int *z = (int *)malloc(total_len * sizeof(int));
    compute_z_function(combined, z, total_len);
    
    int max_len = 0;
    for (int i = len1 + 1; i < total_len; i++) {
        if (z[i] > 0 && i + z[i] == total_len) {
            max_len = z[i];
            break;
        }
    }
    
    free(combined);
    free(z);
    return max_len;
}

int main() {
    char str1[MAX_LEN], str2[MAX_LEN];
    
    if (fgets(str1, MAX_LEN, stdin) == NULL) {
        printf("%d %d\n", 0, 0);
        return 0;
    };

    if (fgets(str2, MAX_LEN, stdin) == NULL){
        printf("%d %d\n", 0, 0);
        return 0;
    };
    
    str1[strcspn(str1, "\n")] = '\0';
    str2[strcspn(str2, "\n")] = '\0';
    
    int result1 = max_prefix_suffix(str1, str2);
    int result2 = max_suffix_prefix(str1, str2);
 
    printf("%d %d\n", result1, result2);
    
    return 0;
}