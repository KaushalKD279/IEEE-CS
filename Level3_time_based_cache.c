#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_ENTRIES 100

typedef struct {
    char key[50];
    char value[50];
    time_t expiryTime;
} CacheEntry;

typedef struct {
    CacheEntry heap[MAX_ENTRIES];
    int size;
} MinHeap;

typedef struct {
    CacheEntry entries[MAX_ENTRIES];
    int size;
} HashMap;

typedef struct {
    MinHeap minHeap;
    HashMap hashMap;
} TimeBasedCache;

void swap(CacheEntry *a, CacheEntry *b) {
    CacheEntry temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyDown(MinHeap *heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    if (left < heap->size && heap->heap[left].expiryTime < heap->heap[smallest].expiryTime)
        smallest = left;
    if (right < heap->size && heap->heap[right].expiryTime < heap->heap[smallest].expiryTime)
        smallest = right;
    if (smallest != index) {
        swap(&heap->heap[index], &heap->heap[smallest]);
        heapifyDown(heap, smallest);
    }
}

void heapifyUp(MinHeap *heap, int index) {
    int parent = (index - 1) / 2;
    while (index > 0 && heap->heap[index].expiryTime < heap->heap[parent].expiryTime) {
        swap(&heap->heap[index], &heap->heap[parent]);
        index = parent;
        parent = (index - 1) / 2;
    }
}

void removeExpired(TimeBasedCache *cache) {
    time_t now = time(NULL);
    while (cache->minHeap.size > 0 && cache->minHeap.heap[0].expiryTime <= now) {
        CacheEntry expired = cache->minHeap.heap[0];
        cache->minHeap.heap[0] = cache->minHeap.heap[cache->minHeap.size - 1];
        cache->minHeap.size--;
        heapifyDown(&cache->minHeap, 0);

        for (int i = 0; i < cache->hashMap.size; i++) {
            if (strcmp(cache->hashMap.entries[i].key, expired.key) == 0) {
                for (int j = i; j < cache->hashMap.size - 1; j++) {
                    cache->hashMap.entries[j] = cache->hashMap.entries[j + 1];
                }
                cache->hashMap.size--;
                break;
            }
        }
    }
}

void set(TimeBasedCache *cache, const char *key, const char *value, int expirySeconds) {
    removeExpired(cache);
    time_t expiryTime = time(NULL) + expirySeconds;
    
    for (int i = 0; i < cache->hashMap.size; i++) {
        if (strcmp(cache->hashMap.entries[i].key, key) == 0) {
            strcpy(cache->hashMap.entries[i].value, value);
            cache->hashMap.entries[i].expiryTime = expiryTime;
            return;
        }
    }
    
    if (cache->hashMap.size < MAX_ENTRIES) {
        strcpy(cache->hashMap.entries[cache->hashMap.size].key, key);
        strcpy(cache->hashMap.entries[cache->hashMap.size].value, value);
        cache->hashMap.entries[cache->hashMap.size].expiryTime = expiryTime;
        cache->minHeap.heap[cache->minHeap.size] = cache->hashMap.entries[cache->hashMap.size];
        cache->hashMap.size++;
        heapifyUp(&cache->minHeap, cache->minHeap.size);
        cache->minHeap.size++;
    }
}

const char* get(TimeBasedCache *cache, const char *key) {
    removeExpired(cache);
    for (int i = 0; i < cache->hashMap.size; i++) {
        if (strcmp(cache->hashMap.entries[i].key, key) == 0) {
            return cache->hashMap.entries[i].value;
        }
    }
    return "None";
}

int main() {
    TimeBasedCache cache = {.minHeap.size = 0, .hashMap.size = 0};
    char key[50], value[50];
    int expiry;
    char choice;
    
    do {
        printf("Enter key: ");
        scanf("%s", key);
        printf("Enter value: ");
        scanf("%s", value);
        printf("Enter expiry time (in seconds): ");
        scanf("%d", &expiry);
        
        set(&cache, key, value, expiry);
        
        printf("Do you want to enter another key-value pair? (y/n): ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');
    
    do {
        printf("Enter key to retrieve: ");
        scanf("%s", key);
        printf("Get %s: %s\n", key, get(&cache, key));
        
        printf("Do you want to retrieve another key? (y/n): ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');
    
    return 0;
}
