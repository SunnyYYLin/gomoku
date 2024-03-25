#ifndef SHAPE_H
#define SHAPE_H

#include "board.h"

#ifndef SHAPE_SIZE
#define SHAPE_SIZE 6
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
            int longs;
            int fives;
            int open_fours;
            int broken_fours;
            int open_threes;
            int jump_threes;
        };
        int arr[SHAPE_SIZE];
    };
} Shape;

// Structure representing a line of pieces on the board
typedef struct {
    int len;            // Length of the line
    int segs[SIZE];     // Segments in the line
    Shape shape;        // The shape formed by the line
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
Line num_jump_threes(Line line);

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