#ifndef POSITION_H
#define POSITION_H

#include "symbol.h"

typedef struct {
    int x;
    int y;
} Position;

typedef Position Directions[4];

extern Directions directions;

int is_empty(Position pos);
int is_color(Position pos, ChessColor color);
int is_center(Position pos);
int is_in_board(Position pos);
int is_on_edge(Position pos);
int is_on_corner(Position pos);
int is_valid(Position pos);
Position rev_direc(Position direction);
Position pos_make(int x, int y);
Position pos_move(Position pos, Position direction);
int is_end(ChessColor color, Position pos, Position direction);
Position move_to_end(ChessColor color, Position pos, Position direction);

#endif // POSITION_H