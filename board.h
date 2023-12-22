#ifndef BOARD_H
#define BOARD_H

#define EMPTY 0
#define BLACK 1
#define WHITE -1

#define SIZE 15 // The size of the board

typedef struct {
    int x;
    int y;
} Position;

void init_board(int*** board);
void print_board(int** board, Position pos_new, int turn);
void drop_board(int** board, Position pos, int color);
void undo_board(int** board, Position pos);
int is_empty(int** board, Position pos);
int is_color(int** board, Position pos, int color);
int is_center(Position pos);
int is_in_board(Position pos);
int is_on_edge(Position pos);
int is_on_corner(Position pos);
int is_valid(int** board, Position pos);
int is_end(int** board, Position pos, Position direction, int color);
Position rev_direc(Position direction);
Position pos_make(int x, int y);
Position pos_move(Position pos_at, Position direction);
Position move_to_end(int** board, Position pos_at, Position direction, int color);

#endif // POSITION_H