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
    int length; 
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
    if (l == NULL || n == NULL || l->head == n || !list_node_present(l, n)) {
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

    return 0; 
}


int list_node_get_value(const struct node *n) {
    if (n == NULL) {
        return 0; 
    }

    return n->item; 
}

int list_node_set_value(struct node *n, int value) {
    /* ... SOME CODE MISSING HERE ... */
}

int list_unlink_node(struct list *l, struct node *n) {
    /* ... SOME CODE MISSING HERE ... */
}

void list_free_node(struct node *n) {
    /* ... SOME CODE MISSING HERE ... */
}

int list_cleanup(struct list *l) {
    /* ... SOME CODE MISSING HERE ... */
}

int list_node_present(const struct list *l, const struct node *n) {
    if (l == NULL || n == NULL) {
        return -1; 
    }
    
    struct node *temp = l->head; 
    struct node *temp1 = l->head;

    while (temp->next_node != NULL) {
        if (temp == n) {
            return 1; 
        }
        
        temp1 = temp;
        temp = temp->next_node; 
    }

    return 0; 
}

int list_insert_after(struct list *l, struct node *n, struct node *m) {
    /* ... SOME CODE MISSING HERE ... */
}

int list_insert_before(struct list *l, struct node *n, struct node *m) {
    /* ... SOME CODE MISSING HERE ... */
}

size_t list_length(const struct list *l) {
    /* ... SOME CODE MISSING HERE ... */
}

struct node *list_get_ith(const struct list *l, size_t i) {
    /* ... SOME CODE MISSING HERE ... */
}

struct list *list_cut_after(struct list *l, struct node *n) {
    /* ... SOME CODE MISSING HERE ... */
}
