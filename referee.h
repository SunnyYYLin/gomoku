# ifndef REFEREE_H
# define REFEREE_H

#include "board.h"

typedef struct {
    int open_threes;
    int broken_threes;
    int open_fours;
    int broken_fours;
    int fives;
    int longs;
} Shape;

typedef struct {
    int segs[SIZE];
    int is_start_open;
    int is_end_open;
    Shape shape;
} Line;



// referee.c
int is_forbidden(ChessBoard board, Chess newChess);

void referee_output(int referee, Chess newChess);

// line.c
Shape empty_shape();
int fill_segs (ChessBoard board, Position pos_at, int color);
Line empty_line();
Line get_line(ChessBoard board, int color, Position pos_at, Position direction);
Line num_fives_and_longs(Line line);
Line num_open_fours(Line line);
Line num_broken_fours(Line line);
Line num_open_threes(Line line);
Line num_broken_threes(Line line);
Line line_shape(Line line);
Shape sum_lines(ChessBoard board, Chess chess);

# endif