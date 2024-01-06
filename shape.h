#ifndef SHAPE_H
#define SHAPE_H

#include "board.h"

#define SEG_END -2 // The end mark of line.seg[]

#ifndef STG_SIZE
#define STG_SIZE 10
#endif

#ifndef SEG_GAP
#define SEG_GAP 2
#endif

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// Structure representing various shapes made by consecutive pieces on the board
typedef struct {
    union {
        struct {
            int longs;          // Number of longs (more than five consecutive pieces)
            int fives;          // Number of fives (five consecutive pieces)
            int open_fours;     // Number of open fours (four consecutive pieces with open ends)
            int broken_fours;   // Number of broken fours (four consecutive pieces but not open on both ends)
            int open_threes;    // Number of open threes (three consecutive pieces with open ends)
            int jump_threes;    // Number of jump threes (three unconsecutive pieces with open ends)
            int broken_threes;  // Number of broken threes (three pieces but one end blocked by opponent)
            int open_twos;      // Number of open twos (two consecutive pieces with open ends)
            int jump_twos;      // Number of jump twos (two unconsecutive pieces with open ends)
            int djump_twos;     // Number of double jump twos (two distantly unconsecutive pieces with both ends open)
        };
        int arr[STG_SIZE];           // Array of shape counts
    };
} Shape;

// Structure representing a line of pieces on the board
typedef struct {
    int len;            // Length of the line
    int segs[SIZE];     // Segments in the line
    Shape shape;        // The shape formed by the line
    int is_open;
} Line;

// Function declarations

// Prints a shape
void print_shape(Shape shape);

// Creates an empty shape struct
Shape empty_shape();

// Creates an empty line struct
Line empty_line();

// Fills segments in a line based on the board's state
int fill_segs(int** board, Position pos_at, int color);

// Retrieves a line of pieces from the board
Line get_line(int** board, int color, Position pos_at, Position direction);

// Analyze lines and count various shapes
Line num_fives_and_longs(Line line);
Line num_open_fours(Line line);
Line num_broken_fours(Line line);
Line num_open_threes(Line line);
Line num_broken_threes(Line line);
Line num_open_twos(Line line);
Line num_jump_twos(Line line);
Line num_djump_twos(Line line);

// Determine the shape of a line
Line line_shape(Line line);

// Analyze all lines starting from a position
Shape* enroll_lines(int** board, Position pos, int color);

// Sum up shapes from multiple lines
Shape sum_lines(Shape* shapes);

// Determine if a shape is forbidden according to game rules
int is_forbidden(Shape shape, int color);

// Determine if a shape results in a win
int is_win(Shape shape);

#endif /* SHAPE_H */