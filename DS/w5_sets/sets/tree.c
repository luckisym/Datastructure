#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

/* C files can be modified anywhere.
 * So you can change or remove these structs to suit your needs. */
struct tree {
    struct node *root;
    int turbo;
};

struct node {
    int data;
    struct node *left_child;
    struct node *right_child;

    /* ... SOME CODE MISSING HERE ... */
};
typedef struct node node;

/* Unique id's for numbering nodes in dot format. */
static int global_node_counter = 0;

/* Helper function: Allocate a new tree node and initialise it with
 * the given parameters. Return a pointer to the new node or NULL on
 * failure. */
static node *make_node(int data) {
    struct node *n = (struct node *)malloc(sizeof(struct node));
    if (n == NULL) {
        return NULL;
    }

    n->data = data;
    n->left_child = NULL;
    n->right_child = NULL;

    return n;
}

static int print_tree_dot_r(node *root, FILE *dotf) {
    int left_id, right_id, my_id = global_node_counter++;

    if (root == NULL) {
        fprintf(dotf, "    %d [shape=point];\n", my_id);
        return my_id;
    }

    fprintf(dotf, "    %d [color=%s label=\"%d\"]\n", my_id, "black", root->data);

    left_id = print_tree_dot_r(root->left_child, dotf);
    fprintf(dotf, "    %d -> %d [label=\"l\"]\n", my_id, left_id);

    right_id = print_tree_dot_r(root->right_child, dotf);
    fprintf(dotf, "    %d -> %d [label=\"r\"]\n", my_id, right_id);

    return my_id;
}

void tree_dot(const struct tree *tree, const char *filename) {
    node *root = tree->root;
    global_node_counter = 0;
    FILE *dotf = fopen(filename, "w");
    if (!dotf) {
        printf("error opening file: %s\n", filename);
        exit(1);
    }
    fprintf(dotf, "digraph {\n");
    if (root) {
        print_tree_dot_r(root, dotf);
    }
    fprintf(dotf, "}\n");
    fclose(dotf);
}

int tree_check(const struct tree *tree) {
    /* ... OPTIONALLY IMPLEMENT TREE CHECKING HERE ... */
    return 0;
}

struct tree *tree_init(int turbo) {
    struct tree *t = (struct tree *)malloc(sizeof(struct tree));
    if (t == NULL) {
        return NULL;
    }

    t->turbo = turbo;
    t->root = NULL;

    return t;
}

int tree_insert(struct tree *tree, int data) {
    if (tree == NULL) {
        return -1;
    }

    if (tree->root == NULL) {
        tree->root = make_node(data);
        return 0;
    }

    if (tree_find(tree, data)) {
        return 1;
    }

    struct node *parent = tree->root;
    struct node *new_node = make_node(data);

    while (1) {
        if (parent->data > data && parent->left_child != NULL) {
            parent = parent->left_child;
        } else if(parent->data > data && parent->left_child == NULL) {
            parent->left_child = new_node;
            return 0;
        }

        if (parent->data < data && parent->right_child != NULL) {
            parent = parent->right_child;
        } else if (parent->data < data && parent->right_child == NULL) {
            parent->right_child = new_node;
            return 0;
        }
    }
}

static int recursive_lookup(struct node *parent, int data) {
    if (parent == NULL) {
        return 0;
    }

    if (parent->data == data) {
        return 1;
    }

    int found_left = recursive_lookup(parent->left_child, data);
    int found_right = recursive_lookup(parent->right_child, data);

    return (found_left || found_right);
}

int tree_find(struct tree *tree, int data) {
    if (tree == NULL || tree->root == NULL) {
        return 0;
    }

    return recursive_lookup(tree->root, data);
}

static void handle_node_deletion(struct node *parent, struct node *n) {
    struct node *left_child = (n->left_child != NULL) ? n->left_child : NULL;
    struct node *right_child = (n->right_child != NULL) ? n->right_child : NULL;

    if (left_child == NULL && right_child == NULL) {
        if (parent->left_child == n) {
            parent->left_child = NULL;
        } else {
            parent->right_child = NULL;
        }

        free(n);
    } else if(left_child != NULL && right_child != NULL) {

    } else {
        struct node *child = (left_child != NULL) ? left_child : right_child;
        if (parent->left_child == n) {
            parent->left_child = child;
        } else {
            parent->right_child = child;
        }

        free(n);
    }
}
int tree_remove(struct tree *tree, int data) {
    if (tree == NULL || !tree_find(tree, data)) {
        return 1;
    }

    if (tree->root->data == data) {
        free(tree->root);
        tree->root = NULL;
        return 0;
    }

    struct node *parent = tree->root;
    struct node *prev = NULL;
    struct node *temp = NULL;

    while (1) {
        temp = parent;

        if (parent->data == data) {
            handle_node_deletion(prev, parent);
            return 0;
        }

        if (parent->data > data && parent->left_child != NULL) {
            parent = parent->left_child;
        }else if (parent->data < data && parent->right_child != NULL) {
            parent = parent->right_child;
        }

        prev = temp;
    }
}

void tree_print(const struct tree *tree) {
    /* ... SOME CODE MISSING HERE ... */
}

static void recursive_node_cleanup(struct node *parent) {
    if (parent == NULL) {
        return;
    }

    recursive_node_cleanup(parent->left_child);
    recursive_node_cleanup(parent->right_child);

    free(parent);
}

void tree_cleanup(struct tree *tree) {
    if (tree == NULL) {
        return;
    }

    recursive_node_cleanup(tree->root);
    free(tree);
}
