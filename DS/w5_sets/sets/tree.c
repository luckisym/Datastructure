/**Author: Saleeman Mahamud
 * Student Number: 14932458
 * Study: Computer Science
 * 
 * This file contains the implementation of a binary search tree (BST) data structure
 * and related operations such as insertion, removal, printing, and cleanup. The BST
 * is implemented using nodes with left and right children, suitable for organizing
 * data in a hierarchical order. The file also includes functions for generating DOT
 * representation of the tree.
 */

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

/** This recursive function traverses the binary tree rooted at the given parent
 * node in a depth-first manner to determine if a node with the specified data
 * value exists. If the data value is found in the tree, the function returns 1;
 * otherwise, it returns 0.
 *
 * in:
 *  - parent Pointer to the root of the subtree or NULL for an empty subtree.
 *  - data The data value to search for in the binary tree.
 * out:
 *  - 1 if the data value is found, 0 otherwise.
 */
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

void handle_deletion(struct tree *tree, struct node *current, struct node *parent) {
    if (current->left_child == NULL) {
        if (parent == NULL) {
            tree->root = current->right_child;
        } else if (parent->left_child == current) {
            parent->left_child = current->right_child;
        } else {
            parent->right_child = current->right_child;
        }
        free(current);
    } else if (current->right_child == NULL) {
        if (parent == NULL) {
            tree->root = current->left_child;
        } else if (parent->left_child == current) {
            parent->left_child = current->left_child;
        } else {
            parent->right_child = current->left_child;
        }
        free(current);
    } else {
        // Node with two children, find the in-order successor
        node *left_child = current->right_child;
        parent = current;

        while (left_child->left_child != NULL) {
            parent = left_child;
            left_child = left_child->left_child;
        }

        // Copy the in-order successor's data to the current node
        current->data = left_child->data;

        // Remove the in-order successor (it has at most one child)
        if (parent->left_child == left_child) {
            parent->left_child = left_child->right_child;
        } else {
            parent->right_child = left_child->right_child;
        }

        free(left_child);
    }
}

int tree_remove(struct tree *tree, int data) {
    if (tree == NULL || tree->root == NULL) {
        return 1;  
    }

    node *current = tree->root;
    node *parent = NULL;

    // Search for the node to be removed
    while (current != NULL && current->data != data) {
        parent = current;
        if (data < current->data) {
            current = current->left_child;
        } else {
            current = current->right_child;
        }
    }

    if (current == NULL) {
        return 1; 
    }

    handle_deletion(tree, current, parent);

    return 0;  
}


/** This recursive function traverses the binary tree rooted at the given parent
 * node in an in-order fashion. It prints the data of each visited node to the
 * standard output, effectively displaying the nodes in ascending order if the
 * tree is a binary search tree (BST).
 *
 * in: 
 *  - parent: Pointer to the root of the subtree or NULL for an empty subtree.
 */
static void in_order_traversal(node *parent) {
    if (parent == NULL) {
        return;
    }

    in_order_traversal(parent->left_child);
    printf("%d\n", parent->data);
    in_order_traversal(parent->right_child);
    
}

void tree_print(const struct tree *tree) {
    if (tree == NULL || tree->root == NULL) {
        return;
    } 
    
    in_order_traversal(tree->root);
}


/** This recursive function traverses the binary tree rooted at the given parent
 * node and deallocates the memory occupied by each node. It performs a post-order
 * traversal, ensuring that child nodes are freed before their parent. The function
 * is suitable for cleaning up the entire tree, releasing all allocated memory.
 *
 * in:
 *  - parent: Pointer to the root of the subtree or NULL for an empty subtree.
 */
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
