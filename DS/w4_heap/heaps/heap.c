/**
 * Auteur: Saleeman Mahamud
 * Studentennummer: 14932458
 * Study: Computer Science
 *
 * Implementation of a priority queue using a min heap (where the value of each node is less than
 * or equal to the values of its children).
 * The priority queue supports insertion and removal of elements with priorities, where the element
 * with the highest priority is dequeued first. The comparison function for priorities is provided
 * during initialization of the priority queue.
*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "prioq.h"

#define ARRAY_INIT_SIZE 100

static struct heap *heap_init(int (*compare)(const void *, const void *)) {
    struct heap *h = (struct heap *)malloc(sizeof(struct heap));
    if (h == NULL) {
        return NULL;
    }

    h->array = array_init(ARRAY_INIT_SIZE);
    if (h->array == NULL) {
        free(h);
        return NULL;
    }

    h->compare = compare;

    return h;
}

prioq *prioq_init(int (*compare)(const void *, const void *)) {
    return heap_init(compare);
}

long int prioq_size(const prioq *q) {
    return (q != NULL) ? array_size(q->array) : -1;
}

static int heap_cleanup(struct heap *h, void free_func(void *)) {
    if (h == NULL) {
        return 1; // return 1 on error
    }

    array_cleanup(h->array, free_func);
    free(h);

    return 0;
}

int prioq_cleanup(prioq *q, void free_func(void *)) {
    return heap_cleanup(q, free_func);
}

static int heap_insert(struct heap *h, void *p) {
    if (h == NULL || p == NULL) {
        return 1; // return 1 on error
    }

    array_append(h->array, p);
    long i = array_size(h->array) - 1;
    long parent_i = (i - 1) / 2;
    void *parent = array_get(h->array, parent_i);

    while (i > 0) {
        if (h->compare(p, parent) > 0) {
            break;
        }

        array_set(h->array, i, parent);
        array_set(h->array, parent_i, p);

        i = parent_i;
        parent_i = (i - 1) / 2;
        parent = array_get(h->array, parent_i);
    }

    return 0;
}

int prioq_insert(prioq *q, void *p) {
    return heap_insert(q, p);
}

/**
 * Perform heap sort on a given heap.
 *
 * This function implements the heap sort algorithm to sort the elements
 * in the provided heap in ascending order to keep/ make it a min heap where the minimum element is
 * at the root.
 *
 * in:
 *  - h: heap to be sorted
 *
 * side effects:
 *  - changes the heap layout
 */
static void heap_sort(struct heap *h) {
    if (h == NULL) {
        return;
    }

    long size = array_size(h->array);
    long i = 0;

    while (i < size / 2) {
        long left_i = 2 * i + 1;
        long right_i = 2 * i + 2;
        long parent_i = i;

        void *left_child = (left_i < size) ? array_get(h->array, left_i) : NULL;
        void *right_child = (right_i < size) ? array_get(h->array, right_i) : NULL;
        void *parent = array_get(h->array, parent_i);

        if (left_child != NULL && h->compare(left_child, parent) < 0) {
            parent_i = left_i;
            parent = array_get(h->array, left_i);
        }

        if (right_child != NULL && h->compare(right_child, parent) < 0) {
            parent_i = right_i;
        }

        if (parent_i != i) {
            void *temp = array_get(h->array, i);

            array_set(h->array, i, array_get(h->array, parent_i));
            array_set(h->array, parent_i, temp);

            i = parent_i;
        } else {
            break;
        }
    }

}

static void *heap_pop(struct heap *h) {
    if (h == NULL || array_size(h->array) == 0) {
        return NULL;
    }

    long firs_index = 0;
    void *first_elem = array_get(h->array, firs_index);
    void *last_elem = array_pop(h->array);

    array_set(h->array, firs_index, last_elem);
    heap_sort(h);

    return first_elem;
}



void *prioq_pop(prioq *q) {
    return heap_pop(q);
}
