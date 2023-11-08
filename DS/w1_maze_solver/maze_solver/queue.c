/* 
   Name: Saleeman Mahamud
   Student Number: 14932458
   Education: Computer Science

   This is a C program that implements a queue data structure with various queue operations.
   A queue is a fundamental data structure in computer science that follows the "first-in, 
   first-out" (FIFO) principle. It is similar to a real-world queue where the first person to enter 
   the queue is the first to leave. In a queue data structure:

    - Elements are added to the rear (enqueue operation).
    - Elements are removed from the front (dequeue operation).
    - It maintains the order in which elements were added, ensuring that the oldest element is 
      always at the front.
*/

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
    size_t max_elements;
};

struct queue *queue_init(size_t capacity) {
    struct queue *q = malloc(sizeof(struct queue));

    if (q == NULL) {
        return NULL; 
    }

    q->max_elements = 0;
    q->push_count = 0;
    q-> pop_count = 0;
    q->front = -1; 
    q->rear = -1; 
    q->data = (int *)malloc(sizeof(int) * capacity);
    q->capacity = capacity;

    if (q->data == NULL) {
        free(q);
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
    if (q == NULL) {
        return;  
    }

    fprintf(stderr, "stats %d %d %ld\n", q->push_count, q->pop_count, q->max_elements);
}

int queue_push(struct queue *q, int e) {
    if (q == NULL || ((q->rear + 1) % (int) q->capacity) == q->front) {
        return 1;  
    }

    size_t size; 

    if (queue_empty(q)) {
        q->rear = 0; 
        q->front = 0; 

        q->data[q->rear] = e; 
    } else {
        q->rear = (q->rear + 1) % (int) q->capacity; 
        q->data[q->rear] = e; 
    }
    
    q->push_count++; 

    size = queue_size(q);

    if ( size >  q->max_elements)  {
        q->max_elements = size;
    }

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

    if (q->front > q->rear || q->front == q->rear) {
        size = (size_t) ((q->front - q->rear) + 1); 
    } else if (q->rear > q->front) {
        size = (size_t) ((q->rear - q->front) + 1); 
    }

    return size; 
}
