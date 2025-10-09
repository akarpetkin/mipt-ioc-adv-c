#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list {
    char word[20];
    struct list *next;
};

struct list* append(struct list *head, const char *word) {
    struct list *new_node = (struct list*)malloc(sizeof(struct list));
    strcpy(new_node->word, word);
    new_node->next = NULL;

    if (head == NULL) {
        return new_node;
    }

    struct list *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
    return head;
}

void sort_list(struct list *head) {
    int swapped;
    struct list *ptr;
    
    if (head == NULL) return;

    do {
        swapped = 0;
        ptr = head;
        while (ptr->next != NULL) {
            if (strcmp(ptr->word, ptr->next->word) > 0) {
                char temp[20];
                strcpy(temp, ptr->word);
                strcpy(ptr->word, ptr->next->word);
                strcpy(ptr->next->word, temp);
                swapped = 1;
            }
            ptr = ptr->next;
        }
    } while (swapped);
}

void free_list(struct list *head) {
    struct list *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void print_list(struct list *head) {
    struct list *current = head;
    while (current != NULL) {
        printf("%s ", current->word);
        current = current->next;
    }
    printf("\n");
}

int main() {
    char input[1000];
    struct list *head = NULL;
    char *token;

    fgets(input, 1000, stdin);

    char *dot = strchr(input, '.');
    if (dot != NULL) {
        *dot = '\0';
    }

    token = strtok(input, " ");
    while (token != NULL) {
        head = append(head, token);
        token = strtok(NULL, " ");
    }
    
    sort_list(head);
    print_list(head);

    free_list(head);
    return 0;
}