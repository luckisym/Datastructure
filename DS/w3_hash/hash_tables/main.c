/**
 * Author: Saleeman Mahamud
 * Student number: 14932458
 * Study: Computer Science
 *
 * This file contains the main program for testing the hash table implementation.
 * It includes functions for cleaning up strings, creating a hash table from a file,
 * performing lookups, and measuring the time it takes to construct a hash table with
 * various parameters.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stddef.h>

#include "array.h"
#include "hash_func.h"
#include "hash_table.h"

#define LINE_LENGTH 256

#define TABLE_START_SIZE 256
#define MAX_LOAD_FACTOR 0.6
#define HASH_FUNCTION hash_fletcher

#define START_TESTS 2
#define MAX_TESTS 2
#define HASH_TESTS 4

/** This function modifies a given string by removing single quotes and converting
 * all alphabetic characters to lowercase and converting all non-alphabetic to a space.
 *
 * in:
 *  - line: A pointer to the string that needs to be cleaned up.
 *
 * side-effect:
 *  - changes the given string
 */
void cleanup_string(char *line) {
    for (char *c = line; *c; ++c) {
        if (*c == '\'') {
            *c = '\0';
        } else {
            *c = isalpha(*c) ? (char)tolower(*c) : ' ';
        }
    }
}

// /* Return a pointer to a heap allocated string with all the characters we
//  * use as word delimiters. Return NULL on failure. */
// static char *calc_delim(void) {
//     const size_t ascii_table_size = 128;
//     char *res = malloc(sizeof(char) * ascii_table_size);
//     if (!res) {
//         return NULL;
//     }

//     int res_index = 0;
//     for (unsigned char c = 1; c < ascii_table_size; c++) {
//         if (!isalpha(c)) {   /* if it's not an alpha it's a delimiter char. */
//             res[res_index++] = (char) c;
//         }
//     }
//     res[res_index] = '\0';
//     return res;
// }

/* Creates a hash table with a word index for the specified file and
 * parameters. Return a pointer to hash table or NULL if an error occured.
 */
static struct table *create_from_file(char *filename,
                               unsigned long start_size,
                               double max_load,
                               unsigned long (*hash_func)(const unsigned char *)) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        return NULL;

    char *line = malloc(LINE_LENGTH * sizeof(char));
    if (!line) {
        fclose(fp);
        return NULL;
    }

    struct table *hash_table = table_init(start_size, max_load, hash_func);
    if (hash_table == NULL) {
        fclose(fp);
        free(line);
        return NULL;
    }

    int line_number = 0;

    while (fgets(line, LINE_LENGTH, fp)) {
        line_number++;
        cleanup_string(line);
        char *token = strtok(line, " \t\n");

        while (token != NULL) {
            table_insert(hash_table, token, line_number);
            token = strtok(NULL, " \t\n");
        }
    }

    fclose(fp);
    free(line);
    return hash_table;
}

/* Reads words from stdin and prints line lookup results per word.
 * Return 0 if succesful and 1 on failure. */
static int stdin_lookup(struct table *hash_table) {
    char *line = malloc(LINE_LENGTH * sizeof(char));
    if (!line) {
        return 1;
    }

    while (fgets(line, LINE_LENGTH, stdin)) {
        cleanup_string(line);
        char *token = strtok(line, " \t\n");
        printf("%s\n", token);

        while (token != NULL) {
            struct array *values = table_lookup(hash_table, token);
            if (values != NULL) {
                for (size_t i = 0; i < array_size(values); ++i) {
                    printf("* %d\n", array_get(values, i));
                }

                printf("\n");
            } else {
                printf("\n");
            }

            token = strtok(NULL, " \t\n");
        }
    }

    free(line);
    return 0;
}

static void timed_construction(char *filename) {
    /* Here you can edit the hash table testing parameters: Starting size,
     * maximum load factor and hash function used, and see the the effect
     * on the time it takes to build the table.
     * You can edit the tested values in the 3 arrays below. If you change
     * the number of elements in the array, change the defined constants
     * at the top of the file too, to change the size of the arrays. */
    unsigned long start_sizes[START_TESTS] = { 2, 65536 };
    double max_loads[MAX_TESTS] = { 0.2, 1.0 };
    unsigned long (*hash_funcs[HASH_TESTS])(const unsigned char *) = { hash_too_simple,
                                                                       hash_fletcher, hash_adler,
                                                                       hash_murmur };

    for (int i = 0; i < START_TESTS; i++) {
        for (int j = 0; j < MAX_TESTS; j++) {
            for (int k = 0; k < HASH_TESTS; k++) {
                clock_t start = clock();
                struct table *hash_table =
                create_from_file(filename, start_sizes[i], max_loads[j], hash_funcs[k]);
                clock_t end = clock();

                printf("Start: %ld\tMax: %.1f\tHash: %d\t -> Time: %ld "
                       "microsecs\n",
                       start_sizes[i], max_loads[j], k, end - start);
                table_cleanup(hash_table);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: %s text_file [-t]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 3 && !strcmp(argv[2], "-t")) {
        timed_construction(argv[1]);
    } else {
        struct table *hash_table =
        create_from_file(argv[1], TABLE_START_SIZE, MAX_LOAD_FACTOR, HASH_FUNCTION);
        if (hash_table == NULL) {
            printf("An error occured creating the hash table, exiting..\n");
            return EXIT_FAILURE;
        }
        if (stdin_lookup(hash_table) != 0) {
            table_cleanup(hash_table);
            return EXIT_FAILURE;
        }
        table_cleanup(hash_table);
    }

    return EXIT_SUCCESS;
}
