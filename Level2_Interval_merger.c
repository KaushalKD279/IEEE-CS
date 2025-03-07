#include <stdio.h>
#include <stdlib.h>

typedef struct Interval {
    int start, end;
    struct Interval* next;
} Interval;

// Function to create a new interval node
Interval* createInterval(int start, int end) {
    Interval* newInterval = (Interval*)malloc(sizeof(Interval));
    newInterval->start = start;
    newInterval->end = end;
    newInterval->next = NULL;
    return newInterval;
}

// Function to add and merge intervals
void addInterval(Interval** head, int start, int end) {
    Interval* newInterval = createInterval(start, end);
    if (*head == NULL) {
        *head = newInterval;
        return;
    }
    
    Interval* prev = NULL;
    Interval* curr = *head;
    
    while (curr != NULL && curr->end < start) {
        prev = curr;
        curr = curr->next;
    }
    
    while (curr != NULL && curr->start <= end) {
        if (curr->start < start) start = curr->start;
        if (curr->end > end) end = curr->end;
        Interval* temp = curr;
        curr = curr->next;
        free(temp);
    }
    
    newInterval->start = start;
    newInterval->end = end;
    newInterval->next = curr;
    
    if (prev == NULL) {
        *head = newInterval;
    } else {
        prev->next = newInterval;
    }
}

// Function to print the merged intervals
void getIntervals(Interval* head) {
    printf("Merged Intervals: ");
    while (head != NULL) {
        printf("[%d, %d] ", head->start, head->end);
        head = head->next;
    }
    printf("\n");
}

int main() {
    Interval* intervals = NULL;
    int choice, start, end;
    
    while (1) {
        printf("\nMenu:\n1. Add Interval\n2. Get Intervals\n3. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("Enter start and end: ");
                scanf("%d %d", &start, &end);
                addInterval(&intervals, start, end);
                break;
            case 2:
                getIntervals(intervals);
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}
