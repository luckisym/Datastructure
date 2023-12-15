/** Author: Saleeman Mahamud
 * Student Number: 14932458
 * Study: Computer Science
 * 
 * This file contains the implementation of a set data structure. The set is
 * implemented using a binary search tree (BST) to efficiently perform operations
 * such as insertion, lookup, and removal of elements. The set can be initialized
 * with an optional turbo mode, which affects the tree structure to optimize for
 * certain scenarios.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "tree.h"

struct set {
    struct tree *members;
};

struct set *set_init(int turbo) {
    struct set *s = (struct set *)malloc(sizeof(struct set));
    if (s == NULL) {
        return NULL;
    }

    s->members = tree_init(turbo);
    if (s->members == NULL) {
        free(s);
        return NULL;
    }

    return s;
}

int set_insert(struct set *s, int num) {
    return (s != NULL) ? tree_insert(s->members, num) : -1;
}

int set_find(struct set *s, int num) {
    return (s != NULL) ? tree_find(s->members, num) : 0;
}

int set_remove(struct set *s, int num) {
    return (s != NULL) ? tree_remove(s->members, num) : 1;
}

void set_cleanup(struct set *s) {
    if (s == NULL) {
        return;
    }
    
    tree_cleanup(s->members);
    free(s);
}

void set_print(const struct set *s) {
    if (s == NULL) {
        return;
    }

    tree_print(s->members);
    
}

int set_verify(const struct set *s) {
    /* ... OPTIONALLY IMPLEMENT SET CHECKING HERE ... */
    return 0;
}
