#include "list.h"
#include "mysort.h"

void sort_descending(struct list *l) {
    if (l == NULL || list_head(l) == NULL || list_next(list_head(l)) == NULL) {
        return; 
    }

    struct node *current = list_next(list_head(l)); 

    while (current != NULL) {
        struct node *prev = list_prev(l, current);
        int current_value = list_node_get_value(current);

        while (prev != NULL && list_node_get_value(prev) < current_value) {
            list_unlink_node(l, current);
            list_insert_before(l, current, prev);
            
            prev = list_prev(l, prev);
        }

        current = list_next(current); 
    }
}

