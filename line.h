#ifndef LINE_H
#define LINE_H

#include "symbol.h"
#include "position.h"

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

int is_in_segs (Line line, int i);
int is_segs_start (int i);
int is_segs_end (Line line, int i);
int is_segs_inside (Line line, int i);
int num_fives (Line line);
int num_fours (Line line);
int num_live_threes (Line line);
int num_longs (Line line);
Shape empty_shape ();
Line empty_line ();
Line get_line (ChessColor color, Position pos, Position direction);
Line line_shape (Line line);
Shape sum_lines (ChessColor color, Position pos);

#endif // LINE_H