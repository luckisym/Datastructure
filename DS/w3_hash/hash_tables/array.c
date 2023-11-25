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
    if (a == NULL || a->array_size <= index) {
        return -1;
    }

    return a->array[index];
}

/* Note: Although this operation might require the array to be resized and
 * copied, in order to make room for the added element, it is possible to do
 * this in such a way that the amortized complexity is still O(1).
 * Make sure your code is implemented in such a way to guarantee this. */
int array_append(struct array *a, int elem) {
    if (a == NULL) {
        return 0;
    }

    if (a->array_size >= a->curr_capacity) {
        a->array = (int *)realloc(a->array, sizeof(int) * (a->curr_capacity * 2));
    }

    a->array[a->array_size] = elem;

    return 1;
}

unsigned long array_size(const struct array *a) {
    /* ... SOME CODE MISSING HERE ... */
}
