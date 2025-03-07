// Level 1: Stack with Min and Max in O(1) Time
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];
    int minStack[MAX_SIZE];
    int maxStack[MAX_SIZE];
    int top;
} Stack;

// Initialize stack
void initStack(Stack* s) {
    s->top = -1;
}

// Check if stack is empty
int isEmpty(Stack* s) {
    return s->top == -1;
}

// Check if stack is full
int isFull(Stack* s) {
    return s->top == MAX_SIZE - 1;
}

// Push operation
void push(Stack* s, int x) {
    if (isFull(s)) {
        printf("Stack overflow!\n");
        return;
    }
    s->data[++s->top] = x;
    s->minStack[s->top] = (s->top == 0) ? x : (x < s->minStack[s->top - 1] ? x : s->minStack[s->top - 1]);
    s->maxStack[s->top] = (s->top == 0) ? x : (x > s->maxStack[s->top - 1] ? x : s->maxStack[s->top - 1]);
    printf("Pushed %d onto the stack.\n", x);
}

// Pop operation
void pop(Stack* s) {
    if (isEmpty(s)) {
        printf("Stack underflow!\n");
        return;
    }
    printf("Popped %d from the stack.\n", s->data[s->top]);
    s->top--;
}

// Get top element
int top(Stack* s) {
    if (isEmpty(s)) {
        printf("Stack is empty!\n");
        return INT_MIN;
    }
    return s->data[s->top];
}

// Get minimum element
int getMin(Stack* s) {
    if (isEmpty(s)) {
        printf("Stack is empty!\n");
        return INT_MIN;
    }
    return s->minStack[s->top];
}

// Get maximum element
int getMax(Stack* s) {
    if (isEmpty(s)) {
        printf("Stack is empty!\n");
        return INT_MIN;
    }
    return s->maxStack[s->top];
}

// Print stack elements
void printStack(Stack* s) {
    if (isEmpty(s)) {
        printf("Stack is empty!\n");
        return;
    }
    printf("Stack elements: ");
    for (int i = 0; i <= s->top; i++) {
        printf("%d ", s->data[i]);
    }
    printf("\n");
}

// Main function
int main() {
    Stack s;
    initStack(&s);
    int choice, value;

    while (1) {
        printf("\nMenu:\n1. Push\n2. Pop\n3. Top\n4. Get Min\n5. Get Max\n6. Print Stack\n7. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value to push: ");
                scanf("%d", &value);
                push(&s, value);
                break;
            case 2:
                pop(&s);
                break;
            case 3:
                printf("Top element: %d\n", top(&s));
                break;
            case 4:
                printf("Minimum element: %d\n", getMin(&s));
                break;
            case 5:
                printf("Maximum element: %d\n", getMax(&s));
                break;
            case 6:
                printStack(&s);
                break;
            case 7:
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
