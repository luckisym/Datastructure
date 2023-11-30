/**
 * Author: Saleeman Mahamud
 * Student Number: 14932458
 * Study: Computer Science
 *
 * This file contains the implementation of a hash table that uses separate
 * chaining for collision resolution. The hash table stores key-value pairs,
 * where the keys are strings, and the values are dynamic arrays of integers.
 * The hash table is dynamically resized when the load factor exceeds a specified
 * maximum load factor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "array.h"
#include "hash_table.h"

#define VALUE_INIT_SIZE 100
#define TABLE_RESIZE_FACTOR 2

struct table {
    /* The (simple) array used to index the table */
    struct node **array;
    /* The function used for computing the hash values in this table */
    unsigned long (*hash_func)(const unsigned char *);

    /* Maximum load factor after which the table array should be resized */
    double max_load_factor;
    /* Capacity of the array used to index the table */
    unsigned long capacity;
    /* Current number of elements stored in the table */
    unsigned long load;
};

/* Note: This struct should be a *strong* hint to a specific type of hash table
 * You may implement other options, if you can build them in such a way they
 * pass all tests. However, the other options are generally harder to code. */
struct node {
    /* The string of characters that is the key for this node */
    char *key;
    /* A resizing array, containing the all the integer values for this key */
    struct array *value;

    /* Next pointer */
    struct node *next;
};

/* Makes a new node and stores the given key with it
 *
 * in:
 * - key: The key you want to associate with the node
 *
 * out:
 * - A pointer to the initialized node, or NULL if memory allocation fails.
 */
struct node *node_init(const char *key) {
    struct node *n = (struct node *)malloc(sizeof(struct node));
    if (n == NULL) {
        return NULL;
    }

    n->key = (char *)malloc((strlen(key) + 1) * sizeof(char));
    if (n->key == NULL) {
        free(n);
        return NULL;
    }

    n->value = array_init(VALUE_INIT_SIZE);
    if (n->value == NULL) {
        free(n->key);
        free(n);
        return NULL;
    }

    strcpy(n->key, key);
    return n;
}

struct table *table_init(unsigned long capacity,
                         double max_load_factor,
                         unsigned long (*hash_func)(const unsigned char *)) {
    struct table *t = (struct table *)malloc(sizeof(struct table));
    if (t == NULL) {
        return NULL;
    }

    t->array = (struct node **)calloc(capacity, sizeof(struct node *));
    if (t->array == NULL) {
        free(t);
        return NULL;
    }

    t->hash_func = hash_func;
    t->max_load_factor = max_load_factor;
    t->capacity = capacity;
    t->load = 0;

    return t;
}

/* Resizes the given table to a new capacity, rehashing existing nodes
 * to fit the new capacity.
 *
 * in:
 * - t: Pointer to the table to be resized.
 * - capacity: New capacity for the table.
 *
 * out:
 * - Returns 1 on successful resize, 0 otherwise (e.g., if memory allocation fails).
 */
int table_resize(struct table *t, unsigned long capacity) {
    if (t == NULL) {
        return 0;
    }

    struct node **new_array = (struct node **)calloc(capacity, sizeof(struct node *));
    if (new_array == NULL) {
        return 0;
    }

    for (unsigned long i = 0; i < t->capacity; i++) {
        struct node *curr_node = t->array[i];

        while (curr_node != NULL) {
            struct node *temp_node = curr_node->next;
            unsigned long new_i = t->hash_func((const unsigned char *)curr_node->key) % capacity;

            curr_node->next = new_array[new_i];
            new_array[new_i] = curr_node;

            curr_node = temp_node;
        }
    }

    free(t->array);
    t->array = new_array;
    t->capacity = capacity;

    return 1;
}

int table_insert(struct table *t, const char *key, int value) {
    if (t == NULL) {
        return 1;
    }

    // Resize the table if the max load factor is hit
    if (table_load_factor(t) >= t->max_load_factor) {
        if (!table_resize(t, t->capacity * TABLE_RESIZE_FACTOR)) {
            return 1;
        }
    }

    struct array *a = table_lookup(t, key);
    if (a != NULL) {
        return (!array_append(a, value)) ? 0 : 1;
    }

    // Node not found, make a new one
    struct node *new_node = node_init(key);
    if (new_node == NULL) {
        return 1;
    }

    unsigned long index = t->hash_func((const unsigned char *)key) % t->capacity;
    array_append(new_node->value, value);

    new_node->next = t->array[index];
    t->array[index] = new_node;
    t->load++;

    return 0;
}

struct array *table_lookup(const struct table *t, const char *key) {
    if (t == NULL) {
        return NULL;
    }

    unsigned long index = t->hash_func((const unsigned char *)key) % t->capacity;
    struct node *current_node = t->array[index];

    while (current_node) {
        if (strcmp(current_node->key, key) == 0) {
            return current_node->value;
        }

        current_node = current_node->next;
    }

    return NULL;
}

double table_load_factor(const struct table *t) {
    return (t != NULL) ? (double) t->load / (double) t->capacity : -1.0;
}

/* Cleans up the memory associated with a node, including its key and value.
 *
 * in:
 * - n: Pointer to the node to be cleaned up. If NULL, no action is taken.
 */
void node_cleanup(struct node *n) {
    if (n == NULL) {
        return;
    }

    free(n->key);
    array_cleanup(n->value);
    free(n);
}

int table_delete(struct table *t, const char *key) {
    if (t == NULL) {
        return -1;
    }

    unsigned long index = t->hash_func((const unsigned char *)key) % t->capacity;
    struct node *current = t->array[index];
    struct node *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                t->array[index] = current->next;
            } else {
                prev->next = current->next;
            }

            node_cleanup(current);
            t->load--;
            return 0;
        }

        prev = current;
        current = current->next;
    }

    return 1;
}


void table_cleanup(struct table *t) {
    if (t == NULL) {
        return;
    }

    for (unsigned long i = 0; i < t->capacity; i++) {
        struct node *curr_node = t->array[i];

        while (curr_node != NULL) {
            struct node *temp_node = curr_node->next;

            node_cleanup(curr_node);
            curr_node = temp_node;
        }
    }

    free(t->array);
    free(t);
}
