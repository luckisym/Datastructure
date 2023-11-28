/* Name: Saleeman Mahamud
 * Student Number: 14932458
 * Education: Computer Science 
 * 
 * This file defines a simple doubly linked list data structure and provides basic operations to 
 * manipulate the list. The list consists of nodes, where each node contains an integer item,
 * a pointer to the next node, and a pointer to the previous node.
 * The list is initialized with a head pointer pointing to the first node.
*/

#include "list.h"

struct node {
    int item;
    struct node *next_node;
    struct node *prev_node; 
};

struct list {
    struct node *head;
};


struct list *list_init(void) {
    struct list *l = (struct list *)malloc(sizeof(struct list));
    if (l == NULL) {
        return NULL; 
    }

    l->head = NULL; 

    return l; 
}

struct node *list_new_node(int num) {
    struct node *n = (struct node *)malloc(sizeof(struct node));
    if (n == NULL) {
        return NULL; 
    }

    n->item = num;
    n->next_node = NULL; 
    n->prev_node = NULL; 

    return n; 
}

struct node *list_head(const struct list *l) {
    return (l != NULL && l->head != NULL) ? l->head : NULL;  
}

struct node *list_next(const struct node *n) {
    return (n != NULL && n->next_node != NULL) ? n->next_node : NULL; 
}

int list_add_front(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) {
        return 1; 
    }
    
    if (l->head == NULL) {
        l->head = n; 
    } else {
        n->next_node = l->head; 
        l->head->prev_node = n; 
        l->head = n; 
    }

    return 0; 
}

struct node *list_tail(const struct list *l) {
    if (l == NULL || l->head == NULL) {
        return NULL;
    }

    struct node *temp = l->head;

    while (temp->next_node != NULL) {
        temp = temp->next_node;
    }

    return temp;
}


struct node *list_prev(const struct list *l, const struct node *n) {
    return (l != NULL && n != NULL && list_node_present(l, n) && n != l->head) ? n->prev_node : NULL; 
}


int list_add_back(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) {
        return 1;
    }

    struct node *last_node = list_tail(l); 
    if (last_node == NULL) {
        l->head = n;
    } else {
        last_node->next_node = n; 
        n->prev_node = last_node;
    }
 
    return 0; 
}


int list_node_get_value(const struct node *n) {
    return (n != NULL) ? n->item : -1; 
}

int list_node_set_value(struct node *n, int value) {
    if (n == NULL) {
        return 1; 
    }

    n->item = value; 
    return 0; 
}

int list_unlink_node(struct list *l, struct node *n) {
    if (l == NULL || n == NULL || !list_node_present(l, n)) {
        return 1;
    }

    struct node *prev_n = n->prev_node;
    struct node *next_n = n->next_node;

    if (n == list_tail(l)) {
        if (prev_n != NULL) {
            prev_n->next_node = NULL;
        }
        n->prev_node = NULL;
        if (n == l->head) {
            l->head = prev_n; 
        }
    } else if (n == list_head(l)) {
        if (next_n != NULL) {
            next_n->prev_node = NULL;
            l->head = next_n;  
        }
        n->next_node = NULL;
    } else {
        if (next_n != NULL) {
            next_n->prev_node = prev_n;
        }
        if (prev_n != NULL) {
            prev_n->next_node = next_n;
        }
    }

    n->next_node = NULL;
    n->prev_node = NULL;
    
    return 0;
}



void list_free_node(struct node *n) {
    if (n != NULL) {
        free(n);
    }
}

int list_cleanup(struct list *l) {
    if (l == NULL) {
        return 1;  
    }

    struct node *current = l->head;
    struct node *next = l->head; 

    while (current != NULL) {
        next = current->next_node;
        list_free_node(current); 
        current = next;
    }

    free(l);  
    return 0;  
}


int list_node_present(const struct list *l, const struct node *n) {
    if (l == NULL || n == NULL) {
        return 0; 
    }

    struct node *temp = l->head;

    while (temp != NULL) {
        if (temp == n) {
            return 1;
        }
        temp = temp->next_node;
    }

    return 0;
}


int list_insert_after(struct list *l, struct node *n, struct node *m) {
    if (l == NULL || n == NULL || m == NULL || !list_node_present(l, m) || list_node_present(l, n)) {
        return 1; 
    }

    if (m == list_tail(l)) {
        m->next_node = n;
        n->prev_node = m;
    } else {
        n->next_node = m->next_node;
        m->next_node->prev_node = n;
        m->next_node = n;
        n->prev_node = m;
    }

    return 0;
}


int list_insert_before(struct list *l, struct node *n, struct node *m) {
    if (l == NULL || n == NULL || m == NULL || !list_node_present(l, m) || list_node_present(l, n)) {
        return 1; 
    }

    if (m == list_head(l)) {
        l->head = n; 
        m->prev_node = n;
        n->next_node = m;
    } else {
        m->prev_node->next_node = n;
        n->prev_node = m->prev_node; 
        m->prev_node = n;
        n->next_node = m;
    }

    return 0;
}

size_t list_length(const struct list *l) {
    if (l == NULL) {
        return 0; 
    }
    
    size_t length = 0; 
    struct node *temp = l->head;

    while (temp != NULL) {
        length++; 
        temp = temp->next_node;
    }

    return length; 
}

struct node *list_get_ith(const struct list *l, size_t i) {
    if (l== NULL || list_length(l) < i) {
        return NULL; 
    }

    struct node *current = l->head; 
    for (size_t i_loop = 0; i_loop < list_length(l); i_loop++) {
        if (i == i_loop) {
            return current; 
        }
        current = current->next_node; 
    }

   return NULL;  
}

struct list *list_cut_after(struct list *l, struct node *n) {
    if (l == NULL || n == NULL || n->next_node == NULL) {
        return NULL;
    }

    struct list *second_half = (struct list *)malloc(sizeof(struct list));
    if (second_half == NULL) {
        return NULL;
    }

    second_half->head = n->next_node;
    n->next_node = NULL;

    return second_half;
}



