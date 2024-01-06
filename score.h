#ifndef SCORE_H
#define SCORE_H

#include "board.h"
#include "shape.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define INFINITY 0xFFFF

extern int** board;

// Strategy = 2*{score_longs, score_five, score_open_four, score_broken_four, score_open_three, score_broken_three, score_open_two, score_broken_two}
typedef struct {
    union {
        int arr[2][STG_SIZE];
        struct {
            int white_score_longs;
            int white_score_five;
            int white_score_open_four;
            int white_score_broken_four;
            int white_score_open_three;
            int white_score_jump_three;
            int white_score_broken_three;
            int white_score_open_two;
            int white_score_jump_two;
            int white_score_djump_two;
            int black_score_longs;
            int black_score_five;
            int black_score_open_four;
            int black_score_broken_four;
            int black_score_open_three;
            int black_score_jump_three;
            int black_score_broken_three;
            int black_score_open_two;
            int black_score_jump_two;
            int black_score_djump_two;
        };
    };
} Strategy;

// Struct defining a position and its score.
typedef struct {
    Position pos; // Position on the board
    int score; // Score for the position
} PosScore;

// Initializes an Strategy with predefined scores.
Strategy init_stg();

// Calculates the score for a given shape.
int score_shape(Shape shape, int color, Strategy stg);

// Evaluates the score of a single move.
int evaluate_move(int** board, Position pos, int color, Strategy stg);

// Evaluates scores for all valid moves on the board.
PosScore* all_posScores(int** board, int color, Position* valid_pos, int valid_count, Strategy stg);

// Sorts the array of PosScore structs (from large to small).
void sort_posScores(PosScore* posScores, int size);

// Returns the most k-ratio valuable positions.
PosScore* mostval_posScores(PosScore* posScores, int size, int* mostval_size, double ratio);

// Prints scores for each valid position on the board.
void print_scores(PosScore* posScores, int valid_count);

int cmpfunc(const void* a, const void* b);

void shuffle_posScores(PosScore* posScores, int size);

#endif // SCORE_H