/* 
   Name: Saleeman Mahamud
   Student Number: 14932458
   Study: Computer Science

   This C program solves a maze using depth-first search (DFS) and marks the path. 
   It reads a maze, finds a path from the start to the destination, and prints the result.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "stack.h"

#define NOT_FOUND -1
#define ERROR -2


/** Check for the next possible position in the maze
 * 
 * in:
 *  - visited: Array of visited positions
 *  - size: size of the array "visited"
 *  - m: the maze 
 *  - x: x coordinate 
 *  - y: y coordinate
 * 
 * out:
 *  - next possible position in the maze
*/
int next_possible_pos(struct maze *m ,int x, int y) {
    int directions[N_MOVES] = {maze_index(m, x - 1, y), maze_index(m, x, y + 1), 
                               maze_index(m, x + 1, y), maze_index(m, x, y - 1)};

    for (int i = 0; i < N_MOVES; i++) {
        int new_x = x + m_offsets[i][0];
        int new_y = y + m_offsets[i][1];

        //Check if the position is valid 
        if (maze_valid_move(m, new_x, new_y) && maze_get(m, new_x, new_y) != WALL &&
            maze_get(m, new_x, new_y) != VISITED) {
            return directions[i]; 
        } 
    }

    return -1; 
    
}

/**Backtracks to the destination and marks the path 
 * 
 * in:
 *  - m: the maze
 *  - stack: the stack 
 * 
 * out:
 *  - the length of the path taken back to the destination
 * 
 * side effects:
 *  - changes the maze layout 
*/
int backtrack_to_des(struct maze *m, struct stack *stack) {
    size_t size = stack_size(stack); 
    int length = 0; 
    
    for (size_t i = 0; i < size; i++) {
        int current_pos = stack_pop(stack); 
        int x = maze_row(m, current_pos); 
        int y = maze_col(m, current_pos); 
        maze_set(m, x, y, PATH); 

        if (!maze_at_destination(m, x, y)) {
            length++;  
        }
    }
    
    return length; 
}
/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occured.
 */
int dfs_solve(struct maze *m) {
    struct stack *stack = stack_init(50000); 
    int x = 0; 
    int y = 0; 
    int steps = 0; 

    maze_start(m, &x, &y); 

    int pos = maze_index(m, x, y); 
    stack_push(stack, pos); 

    while (1) {
        if (stack_empty(stack)) {
            stack_cleanup(stack); 
            return ERROR; 
        }
        
        pos = stack_peek(stack);
        
        y = maze_col(m, pos); 
        x = maze_row(m, pos); 

        maze_set(m, x, y, VISITED);

        if (maze_at_destination(m, x, y)) {
            int length = backtrack_to_des(m, stack); 
            stack_cleanup(stack); 
            return length; 
        }
        
        int next_pos = next_possible_pos(m, x, y); 
        
        // Nothing found 
        if (next_pos == NOT_FOUND) {
            stack_pop(stack); 
        } else { 
            stack_push(stack, next_pos); 
        }

        steps++;
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
