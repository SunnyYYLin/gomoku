#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int **board;

// Initializes the game board with memory allocation.
void init_board(int*** board_ptr) {
    int **board_temp = (int **)malloc(SIZE * sizeof(int *));
    if (board_temp == NULL) {
        // If memory allocation fails, print an error message and exit the program
        fprintf(stderr, "Memory allocation failed for board_temp.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < SIZE; i++) {
        board_temp[i] = (int *)malloc(SIZE * sizeof(int));
        if (board_temp[i] == NULL) {
            // If memory allocation fails, free previously allocated memory and exit
            fprintf(stderr, "Memory allocation failed for board_temp[%d].\n", i);
            for (int j = 0; j < i; j++) {
                free(board_temp[j]); // Free previously allocated rows
            }
            free(board_temp); // Free the row pointer array
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < SIZE; j++) {
            board_temp[i][j] = EMPTY; // Assuming EMPTY is defined
        }
    }

    *board_ptr = board_temp; // Assign the address of the allocated board to the external variable
}

// Creates a deep copy of the game board.
int** copy_board(int** board) {
    int** new_board = malloc(SIZE * sizeof(int*));
    if (new_board == NULL) {
        // Memory allocation failed
        return NULL;
    }

    for (int i = 0; i < SIZE; i++) {
        new_board[i] = malloc(SIZE * sizeof(int));
        if (new_board[i] == NULL) {
            // Memory allocation failed, free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(new_board[j]);
            }
            free(new_board);
            return NULL;
        }
        memcpy(new_board[i], board[i], SIZE * sizeof(int));
    }
    return new_board;
}

// Frees memory allocated for the game board.
void free_board(int** board) {
    for (int i = 0; i < SIZE; i++) {
        free(board[i]);
    }
    free(board);
}

// Displays the current state of the board.
void print_board(int** board, Position pos_new, int turn) {
    printf("Round %d\n", turn);

    printf("Last move: %c%d\n", 'A' + pos_new.y, pos_new.x + 1);

    // Print each row of the board
    for (int i = 0; i < SIZE; i++) {
        printf("%2d", i + 1); // Print row number
        for (int j = 0; j < SIZE; j++) {
            // Print the appropriate character based on the board's state
            switch (board[i][j]) {
                case EMPTY:
                    // Special characters for the board's edges and corners
                    if (i == 0 && j == 0) printf("┌");
                    else if (i == 0 && j == SIZE-1) printf("┐");
                    else if (i == SIZE-1 && j == 0) printf("└");
                    else if (i == SIZE-1 && j == SIZE-1) printf("┘");
                    else if (i == 0) printf("┬");
                    else if (i == SIZE-1) printf("┴");
                    else if (j == 0) printf("├");
                    else if (j == SIZE-1) printf("┤");
                    else printf("┼");
                    break;
                case BLACK:
                    if (i == pos_new.x && j == pos_new.y) {
                        printf("▲");
                    }
                    else {
                        printf("●");
                    }
                    break;
                case WHITE:
                    if (i == pos_new.x && j == pos_new.y) {
                        printf("△");
                    }
                    else {
                        printf("○");
                    }
                    break;
            }
        }
        printf("\n");
    }

    // Print column headers
    printf("  ");
    for (int i = 0; i < SIZE; i++) {
        printf("%c ", 'A' + i);
    }
    printf("\n");
}

// Places a piece on the board if the position is valid.
int drop_board(int** board, Position pos, int color) {
    if (is_valid(board, pos, 0) == 1) {
        board[pos.x][pos.y] = color;
        return 1;
    }
    else {
        printf("drop_board: You can not drop at (%d, %d)!\n", pos.x, pos.y);
        return 0;
    }
}

// Places a piece on the board without checking validity.
int drop_board_f(int** board, Position pos, int color) {
    board[pos.x][pos.y] = color;
    return 1;
}

// Removes a piece from the board.
void undo_board(int** board, Position pos) {
    board[pos.x][pos.y] = EMPTY;
}

// Boolean functions to check various conditions on the board.
int is_empty(int** board, Position pos) {
    int x = pos.x;
    int y = pos.y;

    if (is_in_board(pos) && board[x][y] == EMPTY) {
        return 1;
    }

    return 0;
}

int is_color(int** board, Position pos, int color) {
    int x = pos.x;
    int y = pos.y;

    if (is_in_board(pos) && board[x][y] == color) {
        return 1;
    }

    return 0;
}

int is_center(Position pos) {
    int x = pos.x;
    int y = pos.y;

    if (x == SIZE/2 && y == SIZE/2) {
        return 1;
    }

    return 0;
}

int is_in_board(Position pos) {
    int x = pos.x;
    int y = pos.y;

    if (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
        return 1;
    }

    return 0;
}

int is_on_edge(Position pos) {
    int x = pos.x;
    int y = pos.y;

    if (x == 0 || x == SIZE-1 || y == 0 || y == SIZE-1) {
        return 1;
    }

    return 0;
}

int is_on_corner(Position pos) {
    int x = pos.x;
    int y = pos.y;

    if ((x == 0 || x == SIZE-1) && (y == 0 || y == SIZE-1)) {
        return 1;
    }

    return 0;
}

int is_valid(int** board, Position pos, int is_silent) {
    if (!is_in_board(pos)) {
        if (!is_silent) printf("Out of board! Please drop on the board.\n");
        return -1;
    }
    else if (!is_empty(board, pos)) {
        if (!is_silent) printf("The position is not empty! Please drop on an empty position.\n");
        return -2;
    }
    else if (turn == 0 && !(pos.x == SIZE/2 && pos.y == SIZE/2)) {
        if (!is_silent) printf("The first move must be on the center of the board!\n");
        return -3;
    }
    else {
        return 1;
    }
}

// Close end
int is_cut(int** board, Position pos, Position direction, int color) {
    if (is_color(board, pos_move(pos, direction), -color) // X(?)O
    || !is_in_board(pos_move(pos, direction))) { // X(?)|
        return 1;
    }

    return 0;
}

// Broken end, Open end
int is_broken(int** board, Position pos, Position direction, int color) {
    if (is_empty(board, pos) && !is_color(board, pos_move(pos, direction), color)) { // X(_)?, ?=_, O
        return 1;
    }

    return 0;
}

int is_pos_equal(Position pos1, Position pos2) {
    if (pos1.x == pos2.x && pos1.y == pos2.y) {
        return 1;
    }

    return 0;
}

// Position manipulation functions.
Position rev_direc(Position direction) {
    Position rev_direction = {-direction.x, -direction.y};

    return rev_direction;
}

Position pos_make(int x, int y) {
    Position pos = {x, y};
    return pos;
}

Position pos_move(Position pos_at, Position direction) {
    pos_at.x += direction.x;
    pos_at.y += direction.y;

    return pos_at;
}

// Generates a list of valid positions for a given color.
Position* valid_positions(int** board, int color, int* count) {
    *count = 0;
    Position valid_pos_temp[SIZE*SIZE];

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Position pos = {i, j};
            if (is_valid(board, pos, 1) == 1) {
                valid_pos_temp[*count] = pos;
                (*count) ++;
            }
        }
    }

    // Resize the array to match the actual number of valid positions
    Position* valid_pos = malloc(sizeof(Position)*(*count));
    memcpy(valid_pos, valid_pos_temp, sizeof(Position)*(*count));

    return valid_pos;
}