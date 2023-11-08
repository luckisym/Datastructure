/* 
   Name: Saleeman Mahamud
   Student Number: 14932458
   Education: Computer Science 
   
   A stack is another fundamental data structure in computer science, which follows the "last-in, 
   first-out" (LIFO) principle. In a stack data structure:

   - Elements are added and removed from the top of the stack.
   - The last element added is the first one to be removed.
   - Stacks are often visualized as a vertical structure where items are stacked on top of each 
     other.
*/

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

struct stack {
    int pos;
    int *data;
    size_t capacity;
    int push_count; 
    int pop_count; 
    size_t max_elements;
};

struct stack *stack_init(size_t capacity) {
    struct stack *s = malloc(sizeof(struct stack));

    if (s == NULL) {
        return NULL; 
    }

    s->max_elements = 0; 
    s->pos = -1; 
    s->data = (int *)malloc(sizeof(int) * capacity);
    s->capacity = capacity;
    s->pop_count = 0; 
    s->push_count = 0; 

    if (s->data == NULL) {
        free(s);
        return NULL; 
    }
    
    return s;
}

void stack_cleanup(struct stack *s) {
    if (s == NULL) {
        return;  
    }

    free(s->data);
    free(s); 
}

void stack_stats(const struct stack *s) {
    if (s == NULL) {
        return;  
    }

    fprintf(stderr, "stats %d %d %ld\n", s->push_count, s->pop_count, s->max_elements);
}

int stack_push(struct stack *s, int c) { 
    if (s == NULL) {
        return 1;  
    }

    if (s->pos == (int) s->capacity - 1) {
        return 1;
    }

    s->pos++;
    s->data[s->pos] = c;
    s->push_count++; 

    if (stack_size(s) >  s->max_elements)  {
        s->max_elements = stack_size(s);
    }
    
    return 0;
}

int stack_pop(struct stack *s) {
    if (s == NULL) {
        return -1;  
    }

    if (stack_empty(s)) {
        return -1;
    }

    int item = s->data[s->pos];
    s->pos--;
    s->pop_count++; 
    
    return item; 
}

int stack_peek(const struct stack *s) {
    if (s == NULL) {
        return -1;  
    }

    if (stack_empty(s)) {
        return -1;
    }

    return s->data[s->pos]; 
}

int stack_empty(const struct stack *s) {
    if (s == NULL) {
        return -1;  
    }

    return (s->pos == -1) ? 1 : 0; 
}

size_t stack_size(const struct stack *s) {
    if (s == NULL) {
        return (size_t) -1;  
    }
    return (size_t) s->pos + 1;
}
