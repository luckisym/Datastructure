#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "queue.h"

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
bool contains_index(int *array, int n, int pos) {
    for (int i = 0; i < n; i++) {
        if (array[i] == pos) {
            return true; 
        }
    }
    
    return false; 
}

/**
 * 
 * return the index of the next possible node, if there are none return -1. 
*/
void enqueue_possible_pos(struct queue *queue, int *visited, int size, int *elements_queue, 
                          struct maze *m ,int x, int y, int i, int *previous) {
    
    int i_right = maze_index(m, x + 1, y);
    int i_left = maze_index(m, x - 1, y);
    int i_down = maze_index(m, x, y + 1); 
    int i_up = maze_index(m, x, y - 1); 

    int offset = 0; 

    // Check every direction: right, left, up and down
    if (maze_valid_move(m, x + 1, y) && maze_get(m, x + 1, y) != WALL 
        && !contains_index(visited, size, i_right)
        && !contains_index(elements_queue, size, i_right)) {
        queue_push(queue, i_right);
        elements_queue[i] = i_right;
        previous[i_right] = maze_index(m, x, y);
        offset++;   
    }

    if (maze_valid_move(m, x - 1, y) && maze_get(m, x - 1, y) != WALL 
        && !contains_index(visited, size, i_left)
        && !contains_index(elements_queue, size, i_left)) {
        queue_push(queue, i_left); 
        elements_queue[i + offset] = i_left; 
        previous[i_left] = maze_index(m, x, y); 
        offset++; 
    } 
    
    if (maze_valid_move(m, x, y + 1) && maze_get(m, x, y + 1) != WALL 
        && !contains_index(visited, size, i_down)
        && !contains_index(elements_queue, size, i_down)) {
        queue_push(queue, i_down); 
        elements_queue[i + offset] = i_down;  
        previous[i_down] = maze_index(m, x, y);
        offset++; 
    }
    
    if (maze_valid_move(m, x, y - 1) && maze_get(m, x, y - 1) != WALL 
        && !contains_index(visited, size, i_up)
        && !contains_index(elements_queue, size, i_up)) {
        queue_push(queue, i_up); 
        elements_queue[i + offset] = i_up;  
        previous[i_up] = maze_index(m, x, y);
        offset++; 
    }
}

/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occured.
 */
int bfs_solve(struct maze *m) {
    struct queue *queue = queue_init(5000); 
    int m_size = maze_size(m) * 10;
    int visited[m_size]; 
    int elements_queue[m_size]; 
    int previous[m_size * 20]; 
    int x = 0; 
    int y = 0; 
    int path_length = 0; 

    for (int i = 0; i < m_size; i++) {
        previous[i] = -1; // Initialize to -1 (invalid position)
    }

    maze_start(m, &x, &y); 

    int pos = maze_index(m, x, y); 
    queue_push(queue, pos); 

    while (1) {
        if (path_length == m_size) {
            queue_cleanup(queue); 
            return ERROR; 
        }

        if (queue_empty(queue)) {
            queue_cleanup(queue); 
            return NOT_FOUND; 
        }

        pos = queue_pop(queue);
        
        visited[path_length] = pos; 

        y = maze_col(m, pos); 
        x = maze_row(m, pos); 

        if (maze_at_destination(m, x, y)) {
            int current = pos;
            int shortest_path[m_size];
            int path_length = 0;
            int length = 0;

            while (current != -1) {
                shortest_path[length] = current;
                current = previous[current]; 

                int path_y = maze_col(m, current);
                int path_x = maze_row(m, current);

                if (!maze_at_start(m, path_x, path_y)) {
                    path_length++;
                }
                length++;
            }

            for (int i = 0; i < length; i++) {
                int path_pos = shortest_path[i];
                int path_y = maze_col(m, path_pos);
                int path_x = maze_row(m, path_pos);
                maze_set(m, path_x, path_y, PATH); 
            }
            
            queue_cleanup(queue); 
            return path_length; 
        }
        
        enqueue_possible_pos(queue, visited, m_size, elements_queue, m, x, y, path_length, previous); 
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
