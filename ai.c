#include "ai.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

AI ai1;
AI ai2;

// Initializes an AI struct with predefined scores for different shapes.
AI init_ai() {
    AI ai;
    ai.score_five = 1000;
    ai.score_open_four = 1000;
    ai.score_broken_four = 500;
    ai.score_open_three = 500;
    ai.score_broken_three = 100;
    ai.score_open_two = 10;
    ai.score_broken_two = 1;
    return ai;
}

int minimax(int** board, Position pos, int depth, int alpha, int beta, int color, AI ai, int isMaximizing) {
    int valid_count, mostval_count;
    Position* valid_pos = valid_positions(board, color, &valid_count);
    int* scores = evaluate(board, color, valid_pos, valid_count, ai);
    Position* mostval_pos = mostval_positions(valid_pos, scores, valid_count, ai, 3, &mostval_count);

    if (depth == 0 || game_referee(board, pos, color)) {
        return evaluate_board(valid_pos, scores, valid_count);
    }

    if (isMaximizing) {
        int maxEval = -INFINITY;
        for (int i = 0; i < mostval_count; i++) {
            int** new_board = copy_board(board);
            drop_board(new_board,  mostval_pos[i], -color);
            int eval = minimax(new_board,  mostval_pos[i], depth - 1, alpha, beta, -color, ai, 0);
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            // print_board(new_board, pos, 1);
            free_board(new_board);
            if (beta <= alpha)
                break;
        }
        free(valid_pos);
        return maxEval;
    }
    else {
        int minEval = INFINITY;
        for (int i = 0; i < mostval_count; i++) {
            int** new_board = copy_board(board);
            drop_board(new_board,  mostval_pos[i], -color);
            int eval = minimax(new_board,  mostval_pos[i], depth - 1, alpha, beta, -color, ai, 1);
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            // print_board(new_board, pos, 1);
            free_board(new_board);
            if (beta <= alpha)
                break;
        }
        free(valid_pos);
        return minEval;
    }
}

// Determines the best position for the AI to drop a piece.
Position ai_drop(int** board, int color, AI ai) {
    int max_score = -INFINITY;
    int best_move_index = -1;
    int valid_count;
    Position* valid_pos = valid_positions(board, color, &valid_count);

    for (int i = 0; i < valid_count; i++) {
        int** new_board = copy_board(board);
        drop_board(new_board, valid_pos[i], -color);
        int score = minimax(new_board, valid_pos[i], 2, -INFINITY, INFINITY, -color, ai, 0); // depth can be adjusted
        if (score > max_score) {
            max_score = score;
            best_move_index = i;
        }
        free_board(new_board);
    }

    Position best_move = valid_pos[best_move_index];
    free(valid_pos);
    return best_move;
}