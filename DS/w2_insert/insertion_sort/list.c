#include "list.h"

/*
 * TODO: A lot of code missing here. You will need to add implementations for
 * all the functions described in list.h here.
 *
 * Start by adding the definitions for the list and node structs. You may
 * implement any of the Linked List versions discussed in the lecture, which
 * have some different trade-offs for the functions you will need to write.
 *
 * Note: The function prototypes in list.h assume the most basic Singly Linked
 * List. If you implement some other version, you may not need all of the function
 * arguments for all of the described functions. This will produce a warning,
 * which you can suppress by adding a simple if-statement to check the value
 * of the unused parameter.
 *
 * Also, do not forget to add any required includes at the top of your file.
 */

struct node {
    int item;
    struct node *next_node;
    struct node *prev_node; 
};

struct list {
    size_t length; 
    struct node *head;
};


struct list *list_init(void) {
    struct list *l = (struct list *)malloc(sizeof(struct list));
    if (l == NULL) {
        return NULL; 
    }

    l->length = 0; 
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
    return (l->head != NULL) ? l->head : NULL;  
}

struct node *list_next(const struct node *n) {
    return (n != NULL && n->next_node != NULL) ? n->next_node : NULL; 
}

int list_add_front(struct list *l, struct node *n) {
    if (l == NULL) {
        return 1; 
    }

    if (n == NULL) {
        return 1; 
    }
    
    if (l->head == NULL) {
        l->head = n; 
    } else {
        n->next_node = l->head; 
        l->head->prev_node = n; 
        l->head = n; 
    }

    l->length++; 
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
    if (l == NULL || n == NULL || !list_node_present(l, n)) {
        return NULL; 
    }
    
    if (n == l->head) {
        return NULL;
    }
    return n->prev_node; 
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

    l->length++; 
    return 0; 
}


int list_node_get_value(const struct node *n) {
    if (n == NULL) {
        return 0; 
    }

    return n->item; 
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

    if (l->length > 0) {
        l->length--; 
    }
    
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
    struct node *next;

    while (current != NULL) {
        next = current->next_node;
        free(current);
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
    return (l != NULL) ? l->length : 0; 
}

struct node *list_get_ith(const struct list *l, size_t i) {
    if (l== NULL || l->length < i) {
        return NULL; 
    }

    struct node *current = l->head; 
    for (size_t i_loop = 0; i_loop < l->length; i_loop++) {
        if (i == i_loop) {
            return current; 
        }
        current = current->next_node; 
    }

   return NULL;  
}

struct list *list_cut_after(struct list *l, struct node *n) {
    if (l == NULL || n == NULL || !list_node_present(l, n)) {
        return NULL; 
    }

    size_t count = 0; 

    struct list *second_l = list_init(); 
    if (second_l == NULL) {
        return NULL; 
    }
    
    struct node *current = n->next_node; 
    struct node *temp = n->next_node;

    while (current != NULL) {
        struct node *new_node = list_new_node(list_node_get_value(current));
        if (new_node == NULL) {
            list_cleanup(second_l);
            return NULL;
        }

        list_add_back(second_l, new_node); 
        temp = current->next_node;  
        list_unlink_node(l, current);
        list_free_node(current); 
        current = temp;  
        count++; 
    }

    l->length -= count; 
    n->next_node = NULL; 
    return second_l; 
}



