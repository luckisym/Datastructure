#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "stack.h"

#define NOT_FOUND -1
#define ERROR -2

/**Checks if the given position has been visited before 
 * 
 * in:
 *  - visited: array of visited positions
 *  - n: size of the array 
 *  - pos: current position to be checked 
 * 
 * out:
 *  - true if the given position has been visited
 *  - false if it hasnt
*/
bool has_been_visited(int *visited, int n, int pos) {
    for (int i = 0; i < n; i++) {
        if (visited[i] == pos) {
            return true; 
        }
    }
    
    return false; 
}

/**
 * 
 * return the index of the next possible node, if there are none return -1. 
*/
int next_possible_pos(int *visited, int size, struct maze *m ,int x, int y) {

    // Check every direction: right, left, up and down
    if (maze_valid_move(m, x + 1, y) && 
        !has_been_visited(visited, size, maze_index(m, x + 1, y))) {
        return maze_index(m, x + 1, y); 
    } else if (maze_valid_move(m, x - 1, y) && 
               !has_been_visited(visited, size, maze_index(m, x + 1, y))) {
        return maze_index(m, x - 1, y); 
    } else if (maze_valid_move(m, x, y + 1) && 
               !has_been_visited(visited, size, maze_index(m, x + 1, y))) {
        return maze_index(m, x, y + 1); 
    } else if (maze_valid_move(m, x, y - 1) && 
               !has_been_visited(visited, size, maze_index(m, x + 1, y))) {
        return maze_index(m, x, y - 1); 
    }

    return -1; 
    
}
/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occured.
 */
int dfs_solve(struct maze *m) {
    struct stack *stack = stack_init(5000); 
    int size = maze_size(m) * 2;
    int visited[size]; 
    int x = 0; 
    int y = 0; 
    int path_length = 0; 

    maze_start(m, &x, &y); 

    int pos = maze_index(m, x, y); 
    stack_push(stack, pos); 

    while (1) {
        if (path_length == size) {
            stack_cleanup(stack); 
            return NOT_FOUND; 
        }

        if (stack_empty(stack)) {
            stack_cleanup(stack); 
            return NOT_FOUND; 
        }
        
        pos = stack_peek(stack);
        
        visited[path_length] = pos; 

        x = maze_col(m, pos); 
        y = maze_row(m, pos); 

        if (maze_at_destination(m, x, y)) {
            stack_cleanup(stack); 
            return path_length; 
        }
        
        int next_pos = next_possible_pos(visited, size, m, x, y); 

        // Nothing found 
        if (next_pos == NOT_FOUND) {
            stack_pop(stack); 
        } else {
            stack_push(stack, next_pos); 
        }

        path_length++; 
    }
}

int main(void) {
    /* read maze */
    struct maze *m = maze_read();
    if (!m) {
        printf("Error reading maze\n");
        return 1;
    }

    /* solve maze */
    int path_length = dfs_solve(m);
    if (path_length == ERROR) {
        printf("dfs failed\n");
        maze_cleanup(m);
        return 1;
    } else if (path_length == NOT_FOUND) {
        printf("no path found from start to destination\n");
        maze_cleanup(m);
        return 1;
    }
    printf("dfs found a path of length: %d\n", path_length);

    /* print maze */
    maze_print(m, false);
    maze_output_ppm(m, "out.ppm");

    maze_cleanup(m);
    return 0;
}
