#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK_SIZE 1000

typedef struct {
    int data[MAX_STACK_SIZE];
    int top;
} Stack;

void initStack(Stack* stack) {
    stack->top = -1;
}

int isEmpty(Stack* stack) {
    return stack->top == -1;
}

int isFull(Stack* stack) {
    return stack->top == MAX_STACK_SIZE - 1;
}

void push(Stack* stack, int value) {
    if (isFull(stack)) {
        printf("Stack overflow\n");
        exit(1);
    }
    stack->data[++stack->top] = value;
}

int pop(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack underflow\n");
        exit(1);
    }
    return stack->data[stack->top--];
}

int isOperator(const char* token) {
    return strcmp(token, "+") == 0 || 
           strcmp(token, "-") == 0 || 
           strcmp(token, "*") == 0 || 
           strcmp(token, "/") == 0;
}

int performOperation(int a, int b, const char* operator) {
    switch (operator[0]) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) {
                printf("Division by zero\n");
                exit(1);
            }
            return a / b;
        default:
            printf("Unknown operator: %s\n", operator);
            exit(1);
    }
}

int calc(char* expression) {
    Stack stack;
    initStack(&stack);
    
    char* token = strtok(expression, " ");
    while (token != NULL) {
        if (isdigit(token[0]) || (strlen(token) > 1 && isdigit(token[1]))) {
            // Если токен - число
            int number = atoi(token);
            push(&stack, number);
        } else if (isOperator(token)) {
            // Если токен - оператор
            if (stack.top < 1) {
                printf("Invalid expression: not enough operands\n");
                exit(1);
            }
            
            int b = pop(&stack);
            int a = pop(&stack);
            int result = performOperation(a, b, token);
            push(&stack, result);
        } else {
            printf("Invalid token: %s\n", token);
            exit(1);
        }
        
        token = strtok(NULL, " ");
    }
    
    if (stack.top != 0) {
        printf("Invalid expression: too many operands\n");
        exit(1);
    }
    
    int finalResult = pop(&stack);
    return finalResult;
}

int main() {
    char expression[1000];
    
    if (fgets(expression, sizeof(expression), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    // мусор
    size_t len = strlen(expression);
    if (len > 0) {
        if (expression[len - 1] == '\n') {
            expression[len - 1] = '\0';
        }
        if (expression[len - 2] == '.') {
            expression[len - 2] = '\0';
        }
    }
    
    int result = calc(expression);
    printf("%d\n", result);
    
    return 0;
}