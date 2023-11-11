#include "list.h"
#include "list.c"

void sort_descending(struct list *l) {
    if (l == NULL || l->head == NULL || l->head->next_node == NULL) {
        return; 
    }

    struct node *current = l->head->next_node; 

    while (current != NULL) {
        struct node *prev = current->prev_node;
        int current_value = list_node_get_value(current);

        while (prev != NULL && list_node_get_value(prev) < current_value) {
            list_unlink_node(l, current);
            
            if (prev->prev_node == NULL) {
                l->head->next_node = current;
                current->prev_node = NULL;
            } else {
                list_insert_before(l, current, prev);
            }
            
            prev = prev->prev_node;
        }

        current = current->next_node; 
    }
}

