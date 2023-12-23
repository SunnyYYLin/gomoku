#ifndef SHAPE_H
#define SHAPE_H

#include "board.h"

typedef struct {
    int fives;
    int longs;
    int open_fours;
    int broken_fours;
    int open_threes;
    int broken_threes;
    int open_twos;
    int broken_twos;
} Shape;

typedef struct {
    int is_start_open;
    int is_end_open;
    int segs[SIZE];
    Shape shape;
} Line;

Shape empty_shape();
Line empty_line();
int fill_segs(int** board, Position pos_at, int color);
Line get_line(int** board, int color, Position pos_at, Position direction);
Line num_fives_and_longs(Line line);
Line num_open_fours(Line line);
Line num_broken_fours(Line line);
Line num_open_threes(Line line);
Line num_broken_threes(Line line);
Line line_shape(Line line);
Shape* enroll_lines(int** board, Position pos, int color);
Shape sum_lines(Shape* shapes);
int is_forbidden(Shape shape, int color);
int is_win(Shape shape);

#endif /* SHAPE_H */