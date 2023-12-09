/**
 * Auteur: Saleeman Mahamud
 * Studentennummer: 14932458
 * Study: Computer Science
 *
 * This program simulates a doctor's office where patients are registered, treated, and released.
 * It maintains a priority queue of patients based on either their names or ages.
 * Additional features include sorting patients by age and managing the treatment duration.
 */

#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "prioq.h"

#define BUF_SIZE 1024

static char buf[BUF_SIZE];

struct config {
    /* You can ignore these options until/unless you implement the */
    /* bonus features. */

    /* Set to 1 if -y is specified, 0 otherwise. */
    int year;
};

static int parse_options(struct config *cfg, int argc, char *argv[]);

typedef struct {
    char *name;
    int age;
    int duration;
    bool keep_track_of_duration;
} patient_t;

enum return_type {
    CONTINUE,
    BREAK,
    SUCCES,
    FAILURE
};

/**Creates a new patient with the given attributes, makes space for it and returns the pointer to
 * the given space.
 *
 * in:
 *  - name: name of the patient
 *  - age: age of the patient
 *  - duration: the duration of the patient's treatment
 *
 * out:
 *  - a pointer to new patient
 *
 * side effects:
 *  - allocates memory for the new patient
*/
static patient_t *patient_init(char *name, int age, int duration) {
    if (name == NULL) {
        return NULL;
    }

    patient_t *p = (patient_t *)malloc(sizeof(patient_t));
    if (p == NULL) {
        return NULL;
    }

    p->name = name;
    p->age = age;
    p->duration = duration;
    p->keep_track_of_duration = true;

    if (duration <= 0) {
        p->keep_track_of_duration = false;
    }

    return p;
}

/**Cleans up the allocated memory for a patient
 *
 * in:
 * - obj: the patient to be freed
*/
static void patient_cleanup(void *obj) {
    if (obj == NULL) {
        return;
    }

    patient_t *p = (patient_t *)obj;
    free(p->name);
    free(p);
}

/**Prints the remaining patients in the priority queue while freeing them.
 *
 * in:
 *  - q: the priority queue where the remaining patients are
 *
 * side effects:
 *  - prints the remaing patients to the terminal
*/
static void print_remaining_patients(prioq *q) {
    patient_t *curr = NULL;

    while (prioq_size(q) > 0) {
        curr = (patient_t *) prioq_pop(q);
        printf("%s\n", curr->name);
        patient_cleanup(curr);
    }
}

/** This function uses strtok to tokenize the input string and extract
 * patient information including name, age, and optional duration.
 *
 * in:
 *  - token: Pointer to a string pointer to store the current token.
 *  - name_cpy: Pointer to a string pointer to store the copied name.
 *  - age: Pointer to an integer to store the extracted age.
 *  - duration: Pointer to an integer to store the extracted duration.
 *
 * out:
 * An enum representing the result of the operation:
 *  - CONTINUE: continue in the while loop where the function is called
 *  - BREAK: break in the while loop where the function is called
 *  - FAILURE: there was an error
 *  - SUCCES: there was no error
 *
 * side effects:
 *  - allocates memory for name_cpy
 */
static enum return_type get_patient_info(char **name_cpy, int *age, int *duration) {
    char *token = strtok(buf, " \n");
    if (token == NULL) {
        return CONTINUE;
    }

    if (strcmp(token, ".") == 0) {
        return BREAK;
    }

    *name_cpy = (char *)malloc(sizeof(char) * (strlen(token) + 1));
    if (*name_cpy == NULL) {
        return FAILURE;
    }

    strcpy(*name_cpy, token);

    token = strtok(NULL, " \n");
    if (token == NULL) {
        free(*name_cpy);
        return CONTINUE;
    }

    *age = atoi(token);

    token = strtok(NULL, " \n");
    if (token != NULL) {
        *duration = atoi(token);
    }

    return SUCCES;
}

/**This function reads patient information from the standard input and registers
 * them into the specified queue. The patient information includes name,
 * age, and an optional treatment duration. The function continues reading patient
 * data until a period (.) is encountered.
 *
 * in:
 *  - queue: The priority queue to save the new patients.
 *
 * out:
 *  - EXIT_SUCCESS: The function executed successfully
 *  - EXIT_FAILURE: There was an error
 */
static int register_new_patients(prioq *queue) {
    char *name_cpy;
    int duration = 0, age = 0;

    while (1) {
        char *s = fgets(buf, BUF_SIZE, stdin);
        if (s == NULL) {
            fprintf(stderr, "Unexpected end of file. exiting\n");
            prioq_cleanup(queue, patient_cleanup);
            return EXIT_FAILURE;
        }

        int return_t = get_patient_info(&name_cpy, &age, &duration);
        if (return_t == BREAK) {
            break;
        } else if (return_t == CONTINUE) {
            continue;
        } else if(return_t == FAILURE) {
            return EXIT_FAILURE;
        }

        patient_t *patient = patient_init(name_cpy, age, duration);
        if (patient == NULL) {
            prioq_cleanup(queue, patient_cleanup);
            return EXIT_FAILURE;
        }

        prioq_insert(queue, patient);
    }

    return EXIT_SUCCESS;
}

/**Process patient, if no patient is currently in treatment and the priority queue is not empty,
 * a patient is dequeued and their name is printed(if the duration isnt a factor).
 * If the duration is a factor: if a patient is in treatment and the elapsed time equals the
 * patient's duration, the patient's name is printed.
 *
 * in:
 *  - queue: The priority queue containing patient information.
 *  - t_elapsed: Pointer to the elapsed time.
 *  - patient_duration: Pointer to the duration of the patient in treatment.
 *  - name_curr_p: Pointer to a string to store the name of the patient in treatment.
 *
 * side effects:
 *  - allocates memory for name_curr_p if there no patient getting treated
 */
void process_patient(prioq *queue, int *t_elapsed, int *patient_duration, char **name_curr_p) {
    if (*name_curr_p == NULL && prioq_size(queue) > 0) {
        patient_t *curr_patient = (patient_t *)prioq_pop(queue);

        if (!curr_patient->keep_track_of_duration) {
            printf("%s\n", curr_patient->name);
        } else {
            *patient_duration = curr_patient->duration;
            *name_curr_p = (char *)malloc(sizeof(char) * (strlen(curr_patient->name) + 1));

            strcpy(*name_curr_p, curr_patient->name);
        }

        patient_cleanup(curr_patient);
    }

    if (*name_curr_p != NULL && *t_elapsed == *patient_duration) {
        printf("%s\n", *name_curr_p);
        free(*name_curr_p);

        *name_curr_p = NULL;
        *t_elapsed = 1;
        *patient_duration = 0;
    }
}

static int compare_patient_name(const void *a, const void *b) {
    return strcmp(((const patient_t *) a)->name, ((const patient_t *) b)->name);
}

/**This function compares the age of patient 'a' and 'b'. If their ages are equal,
 * it further compares their names using the compare_patient_name function.
 *
 * in:
 *  - a: patient a
 *  - b: patient b
 *
 * out:
 *  - 0 if ages are equal, and names are also equal.
 *  - A positive value if the age of 'a' is greater than the age of 'b'.
 *  - A negative value if the age of 'a' is less than the age of 'b'.
*/
static int compare_patient_age(const void *a, const void *b) {
    int diffrence = ((const patient_t *)a)->age - ((const patient_t *)b)->age;
    return (diffrence == 0) ? compare_patient_name(a, b) : diffrence;
}

/**This function initializes a patient priority queue based on the program configuration,
 * processes patient registrations, tracks elapsed time, and manages patient treatment cycles.
 * And patient name is outputted after the treatment, the remaining patients after 10 iterations
 * are printed aswell.
*/
int main(int argc, char *argv[]) {
    char *name_curr_p = NULL;
    prioq *queue;
    struct config cfg;
    int t_elapsed = 1;
    int patient_duration = -1;

    if (parse_options(&cfg, argc, argv) != 0) {
        return EXIT_FAILURE;
    }

    if (cfg.year) {
        queue = prioq_init(&compare_patient_age);
    } else {
        queue = prioq_init(&compare_patient_name);
    }

    if (queue == NULL) {
        return EXIT_FAILURE;
    }

    for (int iterations = 0;;) {
        if (register_new_patients(queue)) {
            return EXIT_FAILURE;
        }

        process_patient(queue, &t_elapsed, &patient_duration, &name_curr_p);

        if (name_curr_p != NULL) {
            t_elapsed++;
        }

        printf(".\n");

        if (++iterations == 10) {
            if (name_curr_p != NULL) {
                printf("%s\n", name_curr_p);
                free(name_curr_p);
            }

            print_remaining_patients(queue);
            break;
        }
    }

    prioq_cleanup(queue, patient_cleanup);
    return EXIT_SUCCESS;
}

int parse_options(struct config *cfg, int argc, char *argv[]) {
    memset(cfg, 0, sizeof(struct config));
    int c;
    while ((c = getopt(argc, argv, "y")) != -1) {
        switch (c) {
        case 'y':
            cfg->year = 1;
            break;
        default:
            fprintf(stderr, "invalid option: -%c\n", optopt);
            return 1;
        }
    }
    return 0;
}
