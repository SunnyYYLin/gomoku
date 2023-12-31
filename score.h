#ifndef SCORE_H
#define SCORE_H

#include "board.h"
#include "shape.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int** board;

// Strategy = 2*{score_longs, score_five, score_open_four, score_broken_four, score_open_three, score_broken_three, score_open_two, score_broken_two}
typedef struct {
    union {
        double arr[2][STG_SIZE];
        struct {
            double white_score_longs;
            double white_score_five;
            double white_score_open_four;
            double white_score_broken_four;
            double white_score_open_three;
            double white_score_jump_three;
            double white_score_broken_three;
            double white_score_open_two;
            double white_score_jump_two;
            double white_score_djump_two;
            double white_score_broken_two;
            double black_score_longs;
            double black_score_five;
            double black_score_open_four;
            double black_score_broken_four;
            double black_score_open_three;
            double black_score_jump_three;
            double black_score_broken_three;
            double black_score_open_two;
            double black_score_jump_two;
            double black_score_djump_two;
            double black_score_broken_two;
        };
    };
} Strategy;

// Struct defining a position and its score.
typedef struct {
    Position pos; // Position on the board
    double score; // Score for the position
} PosScore;

// Initializes an Strategy with predefined scores.
Strategy init_stg();

// Calculates the score for a given shape.
double score_shape(Shape shape, int color, Strategy stg);

// Evaluates scores for all valid moves on the board.
PosScore* evaluate_all(int** board, int color, Position* valid_pos, int valid_count, Strategy stg);

// Evaluates the score of a single move.
double evaluate_move(int** board, Position pos, int color, Strategy stg);

PosScore maxval_position(PosScore* pos_scores, int valid_count);

PosScore* mostval_positions(PosScore* pos_scores, int valid_count, Strategy stg, int n, int* mostval_count);

int cmpfunc(const void* a, const void* b);

// Prints scores for each valid position on the board.
void print_scores(PosScore* pos_scores, int valid_count);

#endif // SCORE_H