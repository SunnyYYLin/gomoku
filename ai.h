#ifndef AI_H
#define AI_H

#include "board.h"
#include "shape.h"
extern int** board;

typedef struct {
    int score_five;
    int score_open_four;
    int score_broken_four;
    int score_open_three;
    int score_broken_three;
    int score_open_two;
    int score_broken_two;
} AI;

AI init_ai();
int** copy_board(int** board);
void free_board(int** board);
int score_shape(Shape shape, int color, AI ai);
void print_scores(Position* valid_pos, int* score, int valid_count);
int* evaluate(int** board, int color, Position* valid_pos, int valid_count, AI ai);
int evaluate_move(int** board, Position pos, int color, AI ai);
Position ai_drop(int** board, int color, AI ai);
void ai_train();

#endif /* AI_H */