#ifndef SHAPE_H
#define SHAPE_H

#include "board.h"

// Structure representing various shapes made by consecutive pieces on the board
typedef struct {
    int fives;          // Number of fives (five consecutive pieces)
    int longs;          // Number of longs (more than five consecutive pieces)
    int open_fours;     // Number of open fours (four consecutive pieces with open ends)
    int broken_fours;   // Number of broken fours (four consecutive pieces but not open on both ends)
    int open_threes;    // Number of open threes (three consecutive pieces with open ends)
    int broken_threes;  // Number of broken threes (three consecutive pieces but not open on both ends)
    int open_twos;      // Number of open twos (two consecutive pieces with open ends)
    int broken_twos;    // Number of broken twos (two consecutive pieces but not open on both ends)
} Shape;

// Structure representing a line of pieces on the board
typedef struct {
    int is_start_open;  // Indicates if the start of the line is open
    int is_end_open;    // Indicates if the end of the line is open
    int segs[SIZE];     // Segments in the line
    Shape shape;        // The shape formed by the line
} Line;

// Function declarations

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