/* 
   Name: Saleeman Mahamud
   Student Number: 14932458
   Study: Computer Science

   This C program solves a maze using breadth-first search (BFS) and marks the path. 
   It reads a maze, finds a path from the start to the destination, and prints the result.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "queue.h"

#define NOT_FOUND -1
#define ERROR -2
#define MAX_Q_SIZE 5000


/** Add every possible position to the queue so it can be visited later
 * 
 * in:
 *  - m: the maze
 *  - queue: the queue
 *  - parent: array containing every parent of every position
 *  - x: current x coordinate 
 *  - y: current y coordinate 
 * 
 * side effects:
 *  - pushes every possible position onto the queue
 *  - changes the parent array 
*/
void enqueue_possible_pos(struct maze *m, struct queue *queue, int *parent, int x, int y) {
    int pos = maze_index(m, x, y); 
    int directions[N_MOVES] = {maze_index(m, x - 1, y), maze_index(m, x, y + 1), 
                               maze_index(m, x + 1, y), maze_index(m, x, y - 1)}; 

    for (int i = 0; i < N_MOVES; i++) {
        int new_x = x + m_offsets[i][0]; 
        int new_y = y + m_offsets[i][1];

        if (maze_valid_move(m, new_x, new_y) 
            && maze_get(m, new_x, new_y) != WALL 
            && maze_get(m, new_x, new_y) != VISITED) {

            queue_push(queue, directions[i]);
            parent[directions[i]] = pos;   
        }
    }
}

/**Backtracks to the destination using the parent array and marks the path 
 * 
 * in:
 *  - m: the maze
 *  - parent: array containing every parent of every position
 *  - array_size: size of array parent 
 *  - pos: current position in the maze
 * 
 * out:
 *  - the length of the path taken back to the destination
 * 
 * side effects:
 *  - changes the maze layout 
*/
int backtrack_to_des(struct maze *m, int *parent, size_t array_size, int pos) {
    int current_pos = pos;
    int shortest_path[array_size];
    int path_length = 0;
    int steps = 0;

    // Follow the path, taken to the destination, back to the start and caluclate the path length 
    while (current_pos != -1)  {
        shortest_path[steps] = current_pos;
        current_pos = parent[current_pos];

        int y = maze_col(m, current_pos);
        int x = maze_row(m, current_pos);

        if (!maze_at_start(m, x, y)) {
            path_length++;
        }
        steps++;
    }

    // Mark the path to the destination using the shortest_path array we just filled
    for (int i = 0; i < steps; i++) {
        current_pos = shortest_path[i];

        int y = maze_col(m, current_pos);
        int x = maze_row(m, current_pos);

        maze_set(m, x, y, PATH);
    }

    return path_length; 
}

/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occured.
 */
int bfs_solve(struct maze *m) {
    struct queue *queue = queue_init(MAX_Q_SIZE); 
    if (queue == NULL) {
        return ERROR;
    }
    
    size_t size = (size_t) (maze_size(m) * maze_size(m));
    int parent[size]; 
    int x = 0, y = 0, index = 0; 

    for (size_t i = 0; i < size; i++) {
        parent[i] = -1;
    }

    maze_start(m, &x, &y); 

    int pos = maze_index(m, x, y); 
    queue_push(queue, pos); 
  
    while (1) {
        if (queue_empty(queue)) {
            queue_cleanup(queue); 
            return NOT_FOUND; 
        } else if (queue_size(queue) > MAX_Q_SIZE) {
            queue_cleanup(queue); 
            return ERROR; 
        }
        
        pos = queue_pop(queue);

        x = maze_row(m, pos); 
        y = maze_col(m, pos); 

        maze_set(m, x, y, VISITED);

        if (maze_at_destination(m, x, y)) {
            int length = backtrack_to_des(m, parent, size, pos); 
            queue_cleanup(queue); 
            return length; 
        }
    
        enqueue_possible_pos(m, queue, parent, x, y); 
        index++; 
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
    int path_length = bfs_solve(m);
    if (path_length == ERROR) {
        printf("bfs failed\n");
        maze_cleanup(m);
        return 1;
    } else if (path_length == NOT_FOUND) {
        printf("no path found from start to destination\n");
        maze_cleanup(m);
        return 1;
    }
    printf("bfs found a path of length: %d\n", path_length);

    /* print maze */
    maze_print(m, false);
    maze_output_ppm(m, "out.ppm");

    maze_cleanup(m);
    return 0;
}
