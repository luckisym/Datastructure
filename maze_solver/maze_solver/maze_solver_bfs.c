#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "queue.h"

#define NOT_FOUND -1
#define ERROR -2
#define QUEUE_SIZE 100000

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
void enqueue_possible_pos(struct maze *m, struct queue *queue, int *visited, int *queue_elmts,
                          int *previous, int size, int x, int y) {
    int pos = maze_index(m, x, y); 
    int directions[N_MOVES] = {maze_index(m, x - 1, y), maze_index(m, x, y + 1), 
                               maze_index(m, x + 1, y), maze_index(m, x, y - 1)}; 

    for (int i = 0; i < N_MOVES; i++) {
        int new_x = x + m_offsets[i][0]; 
        int new_y = y + m_offsets[i][1];

        if (maze_valid_move(m, new_x, new_y) 
            && maze_get(m, new_x, new_y) != WALL 
            && !contains_index(visited, size, directions[i])
            && !contains_index(queue_elmts, size, directions[i])) {
            queue_push(queue, directions[i]);
            queue_elmts[pos + i] = directions[i];
            previous[directions[i]] = pos;   
        }
    }
}

int backtrack_to_exit(struct maze *m, int *previous, int pos, int m_size) {
    int current = pos;
    int shortest_path[m_size];
    int path_length = 0;
    int i_a = 0;

    while (current != -1)  {
        shortest_path[i_a] = current;
        current = previous[current];

        int y = maze_col(m, current);
        int x = maze_row(m, current);

        if (!maze_at_start(m, x, y)) {
            path_length++;
        }
        i_a++;
    }

    for (int i = 0; i < i_a; i++) {
        int path_pos = shortest_path[i];
        int path_y = maze_col(m, path_pos);
        int path_x = maze_row(m, path_pos);
        maze_set(m, path_x, path_y, PATH);
    }

    return path_length; 
}

/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occured.
 */
int bfs_solve(struct maze *m) {
    struct queue *queue = queue_init(50000); 
    int size = maze_size(m) * 100;
    int visited[size], queue_elmts[size], previous[size]; 
    int x = 0, y = 0, index = 0; 

    for (int i = 0; i < size; i++) {
        previous[i] = -1;
        visited[i] = -1;
        queue_elmts[i] = -1;
    }

    maze_start(m, &x, &y); 

    int pos = maze_index(m, x, y); 
    queue_push(queue, pos); 

    while (1) {
        if (index == size) {
            queue_cleanup(queue); 
            return ERROR; 
        }

        if (queue_empty(queue)) {
            queue_cleanup(queue); 
            return NOT_FOUND; 
        }

        pos = queue_pop(queue);
        
        visited[index] = pos; 

        x = maze_row(m, pos); 
        y = maze_col(m, pos); 

        if (maze_at_destination(m, x, y)) {
            int length = backtrack_to_exit(m, previous, pos, size); 
            queue_cleanup(queue); 
            return length; 
        }
        
        enqueue_possible_pos(m, queue, visited, queue_elmts, previous, size, x, y); 
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
