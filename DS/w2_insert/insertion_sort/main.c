#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "mysort.h"
#define BUF_SIZE 1024

char buf[BUF_SIZE];

struct config {
    /* You can ignore these options until you implement the
       extra command-line arguments. */

    /* Set to 1 if -d is specified, 0 otherwise. */
    int descending_order;

    // Set to 1 if -c is specified, 0 otherwise.
    int combine;

    // Set to 1 if -o is specified, 0 otherwise.
    int remove_odd;

    /* Set to 1 if -z is specified, 0 otherwise. */
    int zip_alternating;
};

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

    struct list *mylist = list_init();

    while (fgets(buf, BUF_SIZE, stdin)) {
        int value;
        sscanf(buf, "%d", &value);

        // // Additional operations based on command line options
        // if (cfg.remove_odd && value % 2 != 0) {
        //     // Skip odd numbers
        //     continue;
        // }

        struct node *n = list_new_node(value); 
        list_add_back(mylist, n);
    }

    // Sort the list based on command-line options
    if (cfg.combine) {
        if (cfg.descending_order) {
            sort_descending(mylist);
        } 
        //else {
        //     sort_ascending(&mylist);
        // }
    }

    // Print the sorted list
    print_list(mylist);

    // Free the memory used by the list
    list_cleanup(mylist);

    return 0;
}