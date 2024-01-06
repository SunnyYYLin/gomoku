#include "search.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

int minimax(int** board, Position pos, int depth, int alpha, int beta, int color, Strategy stg, int isMaximizing) {
    drop_board_f(board, pos, color);

    int valid_size, mostval_size;
    Position* valid_pos = valid_positions(board, -color, &valid_size);
    PosScore* posSc = all_posScores(board, -color, valid_pos, valid_size, stg);
    free(valid_pos);
    PosScore* mostval_posSc = mostval_posScores(posSc, valid_size, &mostval_size, RATIO);
    free(posSc);

    if (depth == 0 || valid_size == 0) {
        int maxval = (valid_size == 0) ? 0 : mostval_posSc[0].score;
        free(mostval_posSc);
        undo_board(board, pos);
        return maxval;
    }

    int value = isMaximizing ? -INFINITY : INFINITY;
    for (int i = 0; i < mostval_size; i++) {
        mostval_posSc[i].score -= (int)(DAMP*minimax(board, mostval_posSc[i].pos, depth - 1, alpha, beta, -color, stg, !isMaximizing));
        int eval = mostval_posSc[i].score;

        if (isMaximizing) {
            value = max(value, eval);
            alpha = max(alpha, eval);
        } else {
            value = min(value, eval);
            beta = min(beta, eval);
        }

        if (beta <= alpha) {
            // printf("pruning\n");
            break;
        }
    }

    free(mostval_posSc);
    undo_board(board, pos);
    return value;
}

int get_depth(int turn) {
    if (turn<3) {
        return 0;
    }
    else {
        int k = turn - 3;
        return (k<2*DEPTH) ? k/2 : DEPTH;
    }
}

// Determines the best position for the stg to drop a piece.
Position ai_drop(int** board, int color, Strategy stg) {
    int valid_size, mostval_size;
    Position* valid_pos = valid_positions(board, color, &valid_size);
    PosScore* posSc = all_posScores(board, color, valid_pos, valid_size, stg);
    free(valid_pos);
    PosScore* mostval_posSc = mostval_posScores(posSc, valid_size, &mostval_size, RATIO);
    free(posSc);

    for (int i = 0; i < mostval_size; i++) {
        mostval_posSc[i].score -= (int)(DAMP*minimax(board, mostval_posSc[i].pos, get_depth(turn), -INFINITY, INFINITY, color, stg, 0));
    }
    sort_posScores(mostval_posSc, mostval_size);
    Position best_move = mostval_posSc[0].pos;
    
    // print_scores(mostval_posSc, mostval_size);
    // for (int i = 0; i<mostval_count; i++) {
    //     printf("mostval_pos[%d]: %d, %d, %f\n", i, mostval_pos[i].pos.x, mostval_pos[i].pos.y, mostval_pos[i].score);
    // }
    // getchar();
    
    free(mostval_posSc);
    return best_move;
}