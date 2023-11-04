#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct queue {
    int front;
    int rear; 
    int *data; 
    size_t capacity;
    int push_count; 
    int pop_count; 
};

struct queue *queue_init(size_t capacity) {
    struct queue *q = malloc(sizeof(struct queue));

    if (q == NULL) {
        free(q);
        perror("Error: "); // TODO: goede error message
        return NULL; 
    }

    q->front = -1; 
    q->rear = -1; 
    q->data = (int *)malloc(sizeof(int) * capacity);
    q->capacity = capacity;

    if (q->data == NULL) {
        free(q->data);
        perror("Error: "); // TODO: goede error message
        return NULL; 
    }
    
    return q;
}

void queue_cleanup(struct queue *q) {
    if (q == NULL) {
        return;  
    }

    free(q->data); 
    free(q); 
}

void queue_stats(const struct queue *q) {
    
}

int queue_push(struct queue *q, int e) {
    if (q == NULL || ((q->rear + 1) % (int) q->capacity) == q->front) {
        return 1;  
    }

    if (queue_empty(q)) {
        q->rear = 0; 
        q->front = 0; 

        q->data[q->rear] = e; 
    } else {
        q->rear = (q->rear + 1) % (int) q->capacity; 
        q->data[q->rear] = e; 
    }
    
    q->push_count++; 

    return 0;
}

int queue_pop(struct queue *q) {
    if (q == NULL || queue_empty(q)) {
        return -1;  
    }

    int item; 
    
    if (q->rear == q->front) {
        item = q->data[q->front]; 
        q->rear = -1; 
        q->front = -1; 
    } else {
        item = q->data[q->front]; 
        q->front = (q->front + 1) % (int) q->capacity; 
    }

    q->pop_count++; 
    
    return item; 
}

int queue_peek(const struct queue *q) {
    if (q == NULL || queue_empty(q)) {
        return -1;  
    }
    
    return q->data[q->front]; 
}

int queue_empty(const struct queue *q) {
    if (q == NULL) {
        return -1;  
    }

    return (q->front == -1 && q->rear == -1) ? 1 : 0;
}

size_t queue_size(const struct queue *q) {
    if (q == NULL) {
        return (size_t) -1;  
    }

    size_t size; 

    if (q->front > q->rear) {
        size = (size_t) ((q->front - q->rear) + 1); 
    } else if (q->rear > q->front) {
        size = (size_t) ((q->rear - q->front) + 1); 
    }

    return size; 
}
