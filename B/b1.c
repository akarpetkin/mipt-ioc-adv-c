#include <stdio.h>
#include <inttypes.h>

typedef struct list {
    uint64_t address;
    size_t size;
    char comment[64];
    struct list *next;
} list;

size_t totalMemoryUsage(list *head) {
    if (head == NULL) {
        return 0;
    }
    
    size_t total_size = head->size;
    list *current = head->next;
    
    while (current != NULL) {
        total_size += current->size;
        current = current->next;
    }
    
    return total_size;
}