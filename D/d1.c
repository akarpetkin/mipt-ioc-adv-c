#include <stdio.h>
#include <stdlib.h>

typedef struct tree {
    int key;
    struct tree *left, *right;
} tree;

tree* createNode(int data) {
    tree* newNode = (tree*)malloc(sizeof(tree));
    newNode->key = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct QueueItem {
    struct tree* node;
    int hd; // расстояние
    struct QueueItem* next;
};

struct Queue {
    struct QueueItem* front;
    struct QueueItem* rear;
};

struct QueueItem* createQueueItem(struct tree* node, int hd) {
    struct QueueItem* newItem = (struct QueueItem*)malloc(sizeof(struct QueueItem));
    newItem->node = node;
    newItem->hd = hd;
    newItem->next = NULL;
    return newItem;
}

void initQueue(struct Queue* q) {
    q->front = q->rear = NULL;
}

void enqueue(struct Queue* q, struct tree* node, int hd) {
    struct QueueItem* newItem = createQueueItem(node, hd);
    
    if (q->rear == NULL) {
        q->front = q->rear = newItem;
        return;
    }
    
    q->rear->next = newItem;
    q->rear = newItem;
}

struct QueueItem* dequeue(struct Queue* q) {
    if (q->front == NULL) return NULL;
    
    struct QueueItem* temp = q->front;
    q->front = q->front->next;
    
    if (q->front == NULL) q->rear = NULL;
    
    return temp;
}

int isQueueEmpty(struct Queue* q) {
    return q->front == NULL;
}

void btUpView(tree* root) {
    if (!root) return;

    int min_hd = 0, max_hd = 0;
    int size = 1000;
    int* hd_values = (int*)malloc(2 * size * sizeof(int));
    int* visited = (int*)malloc(2 * size * sizeof(int));
    
    for (int i = 0; i < 2 * size; i++) {
        hd_values[i] = -2147483647;
        visited[i] = 0;
    }

    struct Queue q;
    initQueue(&q);
    enqueue(&q, root, 0);

    while (!isQueueEmpty(&q)) {
        struct QueueItem* current = dequeue(&q);
        struct tree* node = current->node;
        int hd = current->hd;
        
        if (hd < min_hd) min_hd = hd;
        if (hd > max_hd) max_hd = hd;
        
        int index = hd + size;
        if (!visited[index]) {
            visited[index] = 1;
            hd_values[index] = node->key;
        }

        if (node->left) {
            enqueue(&q, node->left, hd - 1);
        }
        if (node->right) {
            enqueue(&q, node->right, hd + 1);
        }
        
        free(current);
    }

    for (int i = min_hd + size; i <= max_hd + size; i++) {
        if (visited[i]) {
            printf("%d ", hd_values[i]);
        }
    }
    printf("\n");
    
    free(hd_values);
    free(visited);
}


int main() {
    tree* root = createNode(10);
    root->left = createNode(20);
    root->right = createNode(30);
    root->left->left = createNode(40);
    root->left->right = createNode(60);
    root->right->left = createNode(90);
    root->right->right = createNode(100);

    btUpView(root);

    return 0;
}