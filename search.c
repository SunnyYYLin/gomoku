#include "search.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

double minimax(int** board, Position pos, int depth, int alpha, int beta, int color, Strategy stg, int isMaximizing) {
    int valid_count;
    Position* valid_pos = valid_positions(board, color, &valid_count);
    PosScore* pos_scores = evaluate_all(board, color, valid_pos, valid_count, stg);
    // print_scores(pos_scores, valid_count);

    int mostval_count;
    PosScore* mostval_pos = mostval_positions(pos_scores, valid_count, stg, 4, &mostval_count);

    if (depth == 0 || game_referee(board, pos, color) || valid_count == 0) {
        double board_score = maxval_position(pos_scores, valid_count).score;
        free(pos_scores);
        free(valid_pos);
        return board_score;
    }

    double maxEval = -INFINITY;
    double minEval = INFINITY;
    double eval;
    for (int i = 0; i < mostval_count; i++) {
        int** new_board = copy_board(board);
        // print_board(new_board, mostval_pos[i].pos, 0);
        drop_board(new_board, pos_scores[i].pos, color);
        eval = minimax(new_board, pos_scores[i].pos, depth - 1, alpha, beta, -color, stg, !isMaximizing);
        
        if (isMaximizing) {
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
        } 
        else {
            maxEval = min(minEval, eval);
            beta = min(beta, eval);
        }

        free_board(new_board);
        if (beta <= alpha) break;
    }
    return maxEval;
}

// Determines the best position for the stg to drop a piece.
Position ai_drop(int** board, int color, Strategy stg) {
    int max_score = -INFINITY;
    Position best_move;
    int valid_count, mostval_count;
    Position* valid_pos = valid_positions(board, color, &valid_count);
    PosScore* pos_scores = evaluate_all(board, color, valid_pos, valid_count, stg);
    PosScore* mostval_pos = mostval_positions(pos_scores, valid_count, stg, 5, &mostval_count);

    for (int i = 0; i < mostval_count; i++) {
        int** new_board = copy_board(board);
        drop_board(new_board, mostval_pos[i].pos, color);
        double score = minimax(new_board, mostval_pos[i].pos, 3, -INFINITY, INFINITY, -color, stg, 0);
        
        if (score > max_score) {
            max_score = score;
            best_move = mostval_pos[i].pos;
        }

        free_board(new_board);
    }

    // for (int i = 0; i<mostval_count; i++) {
    //     printf("mostval_pos[%d]: %d, %d, %f\n", i, mostval_pos[i].pos.x, mostval_pos[i].pos.y, mostval_pos[i].score);
    // }

    // best_move = mostval_pos[0].pos;

    free(pos_scores);
    free(valid_pos);
    return best_move;
}