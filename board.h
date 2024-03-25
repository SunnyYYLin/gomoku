#ifndef BOARD_H
#define BOARD_H

#define EMPTY 0        // Represents an empty space on the board
#define BLACK 1        // Represents a black piece on the board
#define WHITE -1       // Represents a white piece on the board

#define SIZE 15        // The size of the board

extern int turn;        // External variable representing the current turn

// Structure to represent a position on the board
typedef struct {
    int x;              // X-coordinate
    int y;              // Y-coordinate
} Position;

// Initializes the game board with memory allocation
void init_board(int*** board);

// Creates a deep copy of the game board
int** copy_board(int** board);

// Frees memory allocated for the game board
void free_board(int** board);

// Displays the current state of the board
void print_board(int** board, Position pos_new, int turn);

// Places a piece on the board if the position is valid
int drop_board(int** board, Position pos, int color);

// Places a piece on the board without checking if the position is valid
int drop_board_f(int** board, Position pos, int color);

// Removes a piece from the board
void undo_board(int** board, Position pos);

// Checks if a position on the board is empty
int is_empty(int** board, Position pos);

// Checks if a position on the board is occupied by a given color
int is_color(int** board, Position pos, int color);

// Checks if a position is the center of the board
int is_center(Position pos);

// Checks if a position is within the bounds of the board
int is_in_board(Position pos);

// Checks if a position is on the edge of the board
int is_on_edge(Position pos);

// Checks if a position is on a corner of the board
int is_on_corner(Position pos);

// Checks if a position is valid for a move
int is_valid(int** board, Position pos, int is_silent);

// Checks if a given position is a close end of a line of pieces of the same color
int is_cut(int** board, Position pos, Position direction, int color);

// Checks if a given position is an open end of a line of pieces of the same color
int is_broken(int** board, Position pos, Position direction, int color);

// Checks if two positions are equal
int is_pos_equal(Position pos1, Position pos2);

// Position manipulation functions
Position rev_direc(Position direction);             // Reverses a given direction
Position pos_make(int x, int y);                    // Creates a Position struct from x and y coordinates
Position pos_move(Position pos_at, Position direction); // Moves a Position in a given direction

// Generates a list of valid positions for a given color
Position* valid_positions(int** board, int color, int* valid_count);

Position* get_empty_positions(int** board, int* empty_count);

#endif // BOARD_H