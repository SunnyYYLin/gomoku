#ifndef AI_H
#define AI_H

#include "board.h"
#include "shape.h"

extern int** board;

// Struct defining the AI with scores for different shapes.
typedef struct {
    int score_five;          // Score for five in a row
    int score_open_four;     // Score for an open four (four in a row with open ends)
    int score_broken_four;   // Score for a broken four (four in a row but not open on both ends)
    int score_open_three;    // Score for an open three (three in a row with open ends)
    int score_broken_three;  // Score for a broken three (three in a row but not open on both ends)
    int score_open_two;      // Score for an open two (two in a row with open ends)
    int score_broken_two;    // Score for a broken two (two in a row but not open on both ends)
} AI;

// Initializes an AI with predefined scores.
AI init_ai();

// Creates a deep copy of the game board.
int** copy_board(int** board);

// Frees memory allocated for the game board.
void free_board(int** board);

// Calculates the score for a given shape.
int score_shape(Shape shape, int color, AI ai);

// Prints scores for each valid position on the board.
void print_scores(Position* valid_pos, int* score, int valid_count);

// Evaluates scores for all valid moves on the board.
int* evaluate(int** board, int color, Position* valid_pos, int valid_count, AI ai);

// Evaluates the score of a single move.
int evaluate_move(int** board, Position pos, int color, AI ai);

// Determines the best position for the AI to make a move.
Position ai_drop(int** board, int color, AI ai);

// Placeholder for a function to train the AI.
void ai_train();

#endif /* AI_H */