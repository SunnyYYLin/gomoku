# ifndef REFEREE_H
# define REFEREE_H

#include "board.h"

typedef struct {
    int live_threes;
    int fours;
    int fives;
    int longs;
} Shape;

typedef struct {
    int segs[SIZE];
    int start_open;
    int end_open;
    Shape shape;
} Line;



// referee.c
int is_forbidden(ChessBoard board, Chess chess, int depth);
void referee_output(int referee, Chess newChess);

// line.c
int is_in_segs (Line line, int i);
int is_segs_start (int i);
int is_segs_end (Line line, int i);
int is_segs_inside (Line line, int i);
int num_fives (Line line);
int num_fours (Line line);
int num_live_threes (Line line);
int num_longs (Line line);
int is_new_seg_start(ChessBoard board, int color, Position pos_at, int is_in_seg, int depth);
int is_new_seg_continue(ChessBoard board, int color, Position pos_at, int is_in_seg, int depth);
int is_new_seg_end(ChessBoard board, int color, Position pos_at, int is_in_seg, int depth);
Shape empty_shape ();
Line empty_line ();
Line get_line (ChessBoard board, int color, Position pos_at, Position direction, int depth);
Line line_shape (Line line, int depth);
Shape sum_lines (ChessBoard board, Chess chess, int depth);

# endif