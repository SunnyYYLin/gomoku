#include "score.h"

Strategy stg1, stg2;

int color_to_index(int color) {
    return color>0;
}

// Initializes an Strategy with predefined scores.
Strategy init_stg() {
    Strategy stg;

    // score for white
    stg.white_score_longs = 1000;
    stg.white_score_five = 1000;
    stg.white_score_open_four = 1000;
    stg.white_score_broken_four = 100;
    stg.white_score_open_three = 10;
    stg.white_score_jump_three = 10;
    stg.white_score_broken_three = 8;
    stg.white_score_open_two = 2;
    stg.white_score_jump_two = 2;
    stg.white_score_djump_two = 1.5;
    stg.white_score_broken_two = 1;

    // score for black
    stg.black_score_longs = 0;
    stg.black_score_five = 1000;
    stg.black_score_open_four = 1000;
    stg.black_score_broken_four = 100;
    stg.black_score_open_three = 10;
    stg.black_score_jump_three = 10;
    stg.black_score_broken_three = 8;
    stg.black_score_open_two = 2;
    stg.black_score_jump_two = 2;
    stg.black_score_djump_two = 1.5;
    stg.black_score_broken_two = 1;

    return stg;
}

// Calculates the score for a given shape.
double score_shape(Shape shape, int color, Strategy stg) {
    double score = 0;
    int color_i = color_to_index(color);
    for (int i = 0; i < STG_SIZE; i++) {
        score += shape.arr[i] * stg.arr[color_i][i];
        // if (shape.arr[i]!=0) {
        //     printf("i=%d, shape.arr[i]=%d, stg.arr[color_i][i]=%f\n", i, shape.arr[i], stg.arr[color_i][i]);
        // }
    }
    return score;
}

// Evaluates scores for all valid moves.
PosScore* evaluate_all(int** board, int color, Position* valid_pos, int valid_count, Strategy stg) {
    PosScore* pos_scores = (PosScore*)malloc(sizeof(PosScore) * valid_count);
    for (int i = 0; i < valid_count; i++) {
        double self_score = evaluate_move(board, valid_pos[i], color, stg);
        double opponent_score = evaluate_move(board, valid_pos[i], -color, stg);
        double score = 2*self_score + opponent_score;
        pos_scores[i] = (PosScore){valid_pos[i], score};
        // printf("score[%d]:%f\n", i, score);
    }

    return pos_scores;
}

// Evaluates the score of a single move.
double evaluate_move(int** board, Position pos, int color, Strategy stg) {
    int** board_copy = copy_board(board);
    drop_board(board_copy, pos, color);

    int color_i = color_to_index(color);
    double score = 0;
    Shape* shapes = enroll_lines(board_copy, pos, color);
    Shape shape = sum_lines(shapes);
    if (is_win(shape)) {
        score = 10*stg.arr[color_i][1]; // score_fives
    }
    else if (is_forbidden(shape, color)) {
        score = -10*stg.arr[color_i][1]; // score_fives
    }
    else {
        score = score_shape(shape, color, stg);
    }

    free_board(board_copy);
    return score;
}

PosScore maxval_position(PosScore* pos_scores, int valid_count) {
    double maxval = 0;
    PosScore maxval_pos_score;
    
    for (int i=0; i<valid_count; i++) {
        if (pos_scores[i].score > maxval) {
            maxval = pos_scores[i].score;
            maxval_pos_score = pos_scores[i];
        }
    }

    return maxval_pos_score;
}

// Function to shuffle the array.
void shuffle(PosScore *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        PosScore temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

PosScore* mostval_positions(PosScore* pos_scores, int valid_count, Strategy stg, int n, int* mostval_count) {
    if (n > valid_count) n = valid_count;
    *mostval_count = n;

    shuffle(pos_scores, valid_count);

    qsort(pos_scores, valid_count, sizeof(PosScore), cmpfunc);

    PosScore* mostval_pos_scores = malloc(sizeof(PosScore) * n);
    memcpy(mostval_pos_scores, pos_scores, sizeof(PosScore) * n);

    return mostval_pos_scores;
}

// Compares two PosScore structs.
int cmpfunc(const void* a, const void* b) {
    PosScore* posScoreA = (PosScore*)a;
    PosScore* posScoreB = (PosScore*)b;
    if (posScoreA->score < posScoreB->score) return 1;
    if (posScoreA->score > posScoreB->score) return -1;
    return 0;
}

// Prints scores for each valid position on the board.
void print_scores(PosScore* pos_scores, int valid_count) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Position pos = pos_make(i, j);
            int found = 0;
            for (int k = 0; k < valid_count; k++) {
                if (is_pos_equal(pos, pos_scores[k].pos)) {
                    printf("%f ", pos_scores[k].score);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("X ");
            }
        }
        printf("\n");
    }
}