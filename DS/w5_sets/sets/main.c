/** Author: Saleeman Mahamud
 *  Student Number: 14932458
 *  Study: Computer Science
 * 
 *  File: set_operations.c
 *  Description: This program provides a command-line interface for performing
 *  basic set operations (insertion, removal, lookup, and printing) on a set data
 *  structure. The program reads commands from the standard input, where each command
 *  is a line with an operation symbol ('+', '-', '?', 'p') followed by an optional
 *  integer operand.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"

#define BUF_SIZE 256

void cleanup_and_fail(char *buf, struct set *s) {
    if (s) {
        set_cleanup(s);
    }
    free(buf);
    exit(EXIT_FAILURE);
}

/** This function checks if the specified number exists in the given set. If the
 * number is found in the set, it prints a message indicating that the number
 * was found; otherwise, it prints a message indicating that the number was not found.
 *
 * in:
 *  - s Pointer to the set in which the lookup is performed.
 *  - num The number to look up in the set.
 */
static void look_up_and_print(struct set *s, int num) {
    if (s == NULL) {
        return;
    }

    if (set_find(s, num)) {
        printf("found: %d\n", num);
    } else {
        printf("not found: %d\n", num);
    }
}

int main(void) {
    char *buf = malloc(BUF_SIZE);
    if (!buf) {
        perror("Could not allocate input buffer");
        return EXIT_FAILURE;
    }
    struct set *s = set_init(0);   /* initialize set with turbo turned off. */
    if (s == NULL) {
        perror("Could not allocate set");
        return EXIT_FAILURE;
    }
    
    while (fgets(buf, BUF_SIZE, stdin)) {
        char *endptr;
        char *command;
        char *num_str;
        int num = 0;

        command = strtok(buf, " ");     /* get command: +,-,?,p */
        if (strchr("+-?", *command)) {  /* operation with operand */
            num_str = strtok(NULL, "\n");
            if (!num_str) {
                printf("set operation '%c' requires integer operand\n", *command);
                cleanup_and_fail(buf, s);
            }
            num = (int) strtol(num_str, &endptr, 10);
            if (*endptr != '\0') {
                printf("error converting '%s' to an integer\n", num_str);
                cleanup_and_fail(buf, s);
            }
        }
        switch (*command) {
        case '+':
            set_insert(s, num);
            break;
        case '-':
            set_remove(s, num);
            break;
        case '?':
            look_up_and_print(s, num);
            break;
        case 'p':
            set_print(s);
            break;
        }
    }

    if (set_verify(s)) { /* debug function */
        fprintf(stderr, "Set implementation failed verification!\n");
    }
    free(buf);
    set_cleanup(s);
    return EXIT_SUCCESS;
}
