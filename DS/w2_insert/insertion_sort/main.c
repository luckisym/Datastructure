/* Name: Saleeman Mahamud
 * Student Number: 14932458
 * Education: Computer Science 
 * 
 * This program reads integer values from stdin, performs various sorting operations on a linked 
 * list, and outputs the sorted list based on the specified command-line options.
 * - Command type: -d, Sort Type: Descending Order
 * - Command type: -c, Sort Type: Combine Adjacent Nodes
 * - Command type: -o, Sort Type: Remove Odd Numbers
 * - Command type: -z, Sort Type: Zip Alternating Nodes
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

#define BUF_SIZE 1024

char buf[BUF_SIZE];

struct config {
    int descending_order;
    int combine;
    int remove_odd;
    int zip_alternating;
};

/* Sorts the list using insertion sort.
 * in: 
 *  - l: the list
 *  - descending: true or false depending on the type of sort you want
 * 
 * side effect: 
 *  - changes the list
*/
void sort_list(struct list *l, int descending) {
    if (l == NULL || list_head(l) == NULL || list_next(list_head(l)) == NULL) {
        return; 
    }

    struct node *current = list_next(list_head(l)); 

    while (current != NULL) {
        struct node *prev = list_prev(l, current);
        int current_value = list_node_get_value(current);

        while (
            (descending && prev != NULL && list_node_get_value(prev) < current_value) ||
            (!descending && prev != NULL && list_node_get_value(prev) > current_value)
        ) {
            list_unlink_node(l, current);
            list_insert_before(l, current, prev);
            prev = list_prev(l, current);
        }
        
        current = list_next(current);
    }
}

/* Combines adjacent nodes in the list by adding their values.
 * in: 
 *  - l: the list
 * 
 * side effect: 
 *  - changes the list
*/
void pair_list(struct list *l) {
    if (l == NULL || list_head(l) == NULL) {
        return;
    }

    struct node *current = list_head(l);

    while (current != NULL) {
        struct node *next = list_next(current);
        if (next == NULL) {
            break;
        }

        int current_value = list_node_get_value(current);
        int next_value = list_node_get_value(next);

        list_unlink_node(l, current);
        list_free_node(current);

        list_node_set_value(next, (current_value + next_value));
        current = list_next(next);
    }
}

/* Cuts the list in half and combines corresponding nodes from both halves.
 * in: 
 *  - l: the list
 * 
 * side effect: 
 *  - changes the list
*/
void cut_and_combine(struct list *l) {
    size_t l_length = list_length(l); 
    if ( l_length <= 2) {
        return; 
    }

    size_t cut_index = (l_length % 2 == 0) ? l_length / 2 : (l_length + 1) / 2;
    
    struct node *nn = list_get_ith(l, cut_index - 1);
    struct list *new_l = list_cut_after(l, nn);
    struct node *n_new = list_head(new_l);
    struct node *temp = n_new;
    struct node *n = list_head(l);

    for (size_t i = 0; i < cut_index; i++) {
        temp = list_next(n_new);

        list_unlink_node(new_l, n_new);
        list_insert_after(l, n_new, n);

        n_new = temp;
        n = list_next(list_next(n)); 
    }

    list_cleanup(new_l);
}

int parse_options(struct config *cfg, int argc, char *argv[]) {
    memset(cfg, 0, sizeof(struct config));
    int c;
    while ((c = getopt(argc, argv, "dcoz")) != -1) {
        switch (c) {
        case 'd':
            cfg->descending_order = 1;
            break;
        case 'c':
            cfg->combine = 1;
            break;
        case 'o':
            cfg->remove_odd = 1;
            break;
        case 'z':
            cfg->zip_alternating = 1;
            break;
        default:
            fprintf(stderr, "invalid option: -%c\n", optopt);
            return 1;
        }
    }
    return 0;
}

/* Prints the values of nodes in the list.
 * in: 
 *  - l: the list
 * 
 * side effect: 
 *  - Prints the values of the nodes
*/
void print_list(struct list *l) {
    struct node *current = list_head(l);
    while (current != NULL) {
        printf("%d\n", list_node_get_value(current));
        current = list_next(current);
    }
}

int main(int argc, char *argv[]) {
    struct config cfg;
    if (parse_options(&cfg, argc, argv) != 0) {
        return 1;
    }

    struct list *l = list_init(); 
    int passed = 0; 
    while (fgets(buf, BUF_SIZE, stdin) != NULL) {
        char *token = strtok(buf, " \t\n");
        while (token != NULL) {
            passed = 1; 
            int value = atoi(token);
            
            if (!cfg.remove_odd || (cfg.remove_odd && value % 2 == 0)) {
                struct node *n = list_new_node(value);
                list_add_back(l, n);
            }
    
            token = strtok(NULL, " \t\n");
        }
    }

    if (!passed) {
        list_cleanup(l); 
        return 0; 
    }
    
    sort_list(l, cfg.descending_order);

    if (cfg.combine) {
        pair_list(l); 
    } 
    
    if (cfg.zip_alternating) {
        cut_and_combine(l); 
    }
    
    print_list(l);

    list_cleanup(l); 
    return 0;
}

