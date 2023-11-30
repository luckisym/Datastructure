/**
 * Author: Saleeman Mahamud
 * Student Number: 14932458
 * Study: Computer Science
 *
 * This file contains the declaration of a dynamic array structure for
 * integers and functions to manipulate the array, such as initialization,
 * cleanup, element retrieval, appending, and size inquiry.
 */

#include <stdlib.h>

#include "array.h"

struct array {
    int *array;
    unsigned long curr_capacity;
    unsigned long array_size;
};

struct array *array_init(unsigned long initial_capacity) {
    struct array *a = (struct array *)malloc(sizeof(struct array));
    if (a == NULL) {
        return NULL;
    }

    a->array = (int *)malloc(sizeof(int) * initial_capacity);
    if (a->array == NULL) {
        free(a);
        return NULL;
    }

    a->curr_capacity = initial_capacity;
    a->array_size = 0;

    return a;
}

void array_cleanup(struct array *a) {
    if (a == NULL) {
        return;
    }

    free(a->array);
    free(a);
}

int array_get(const struct array *a, unsigned long index) {
    return (a != NULL && array_size(a) >= index) ? a->array[index] : -1;
}

/* Note: Although this operation might require the array to be resized and
 * copied, in order to make room for the added element, it is possible to do
 * this in such a way that the amortized complexity is still O(1).
 * Make sure your code is implemented in such a way to guarantee this. */
int array_append(struct array *a, int elem) {
    if (a == NULL) {
        return 1;
    }

    if (array_size(a) >= a->curr_capacity) {
        a->curr_capacity *= 2;

        int *new_pointer = (int *)realloc(a->array, sizeof(int) * a->curr_capacity);
        if (new_pointer == NULL) {
        return 1;
        }

        a->array = new_pointer;
    }

    a->array[a->array_size] = elem;
    a->array_size++;

    return 0;
}

unsigned long array_size(const struct array *a) {
    return (a != NULL) ? a->array_size : 0;
}
