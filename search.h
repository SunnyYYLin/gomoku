#ifndef AI_H
#define AI_H

#include "board.h"
#include "shape.h"
#include "game.h"
#include "score.h"
#include <limits.h>

extern int** board;

#define INFINITY 10000


// Evaluates the score of a single line of pieces in a given direction.
double minimax(int** board, Position pos, int depth, int alpha, int beta, int color, Strategy stg, int isMaximizing);

// Determines the best position for the AI to make a move.
Position ai_drop(int** board, int color, Strategy stg);

#endif /* AI_H */