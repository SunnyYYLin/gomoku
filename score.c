#include "score.h"

Strategy stg1, stg2;

int color_to_index(int color) {
    return color>0;
}

// Initializes an Strategy with predefined scores.
Strategy init_stg() {
    Strategy stg;

    // score for white
    stg.white_score_longs = 2000;
    stg.white_score_five = 2000;
    stg.white_score_open_four = 1000;
    stg.white_score_broken_four = 400;
    stg.white_score_open_three = 500;
    stg.white_score_jump_three = 250;
    stg.white_score_broken_three = 250;
    stg.white_score_open_two = 120;
    stg.white_score_jump_two = 50;
    stg.white_score_djump_two = 20;

    // score for black
    stg.black_score_longs = 0;
    stg.black_score_five = 2000;
    stg.black_score_open_four = 1000;
    stg.black_score_broken_four = 400;
    stg.black_score_open_three = 500;
    stg.black_score_jump_three = 250;
    stg.black_score_broken_three = 250;
    stg.black_score_open_two = 120;
    stg.black_score_jump_two = 50;
    stg.black_score_djump_two = 20;

    return stg;
}

// Calculates the score for a given shape.
int score_shape(Shape shape, int color, Strategy stg) {
    int score = 0;
    int color_i = color_to_index(color);
    for (int i = 0; i < STG_SIZE; i++) {
        score += shape.arr[i] * stg.arr[color_i][i];
        // if (shape.arr[i]!=0) {
        //     printf("i=%d, shape.arr[i]=%d, stg.arr[color_i][i]=%f\n", i, shape.arr[i], stg.arr[color_i][i]);
        // }
    }
    return score;
}

// Evaluates the score of a single move.
int evaluate_move(int** board, Position pos, int color, Strategy stg) {
    int** board_copy = copy_board(board);
    drop_board(board_copy, pos, color);

    int color_i = color_to_index(color);
    int score = 0;
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

// Evaluates scores for all valid moves.
PosScore* all_posScores(int** board, int color, Position* valid_pos, int valid_count, Strategy stg) {
    PosScore* pos_scores = (PosScore*)malloc(sizeof(PosScore) * valid_count);
    for (int i = 0; i < valid_count; i++) {
        int self_score = evaluate_move(board, valid_pos[i], color, stg);
        int opponent_score = evaluate_move(board, valid_pos[i], -color, stg);
        int score = 0.9*self_score + opponent_score;
        pos_scores[i] = (PosScore){valid_pos[i], score};
        // printf("score[%d]:%f\n", i, score);
    }

    return pos_scores;
}

// Sorts the array of PosScore structs (from large to small).
void sort_posScores(PosScore* posScores, int size) {
    shuffle_posScores(posScores, size);

    qsort(posScores, size, sizeof(PosScore), cmpfunc);

    return;
}

// Returns the most k-ratio valuable positions.
PosScore* mostval_posScores(PosScore* posScores, int size, int* mostval_size, double ratio) {
    sort_posScores(posScores, size);

    int count = 0;
    PosScore* buff_posScores = (PosScore*)malloc(sizeof(PosScore) * size);
    int threshold = (int)(ratio*posScores[0].score);
    for (int i = 0; i < size; i++) {
        if (posScores[i].score>=threshold) {
            buff_posScores[count] = posScores[i];
            count++;
        }
        else {
            break;
        }
    }

    *mostval_size = count;
    PosScore* mostval_posScores = (PosScore*)malloc(sizeof(PosScore) * count);
    memcpy(mostval_posScores, buff_posScores, sizeof(PosScore) * count);

    return mostval_posScores;
}

// Prints scores for each valid position on the board.
void print_scores(PosScore* pos_scores, int valid_count) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Position pos = pos_make(i, j);
            int found = 0;
            for (int k = 0; k < valid_count; k++) {
                if (is_pos_equal(pos, pos_scores[k].pos)) {
                    printf("%5d", pos_scores[k].score);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("    X");
            }
        }
        printf("\n");
    }
}

// Compares two PosScore structs.
int cmpfunc(const void* a, const void* b) {
    PosScore* posScoreA = (PosScore*)a;
    PosScore* posScoreB = (PosScore*)b;
    if (posScoreA->score < posScoreB->score) return 1;
    if (posScoreA->score > posScoreB->score) return -1;
    return 0;
}

// Function to shuffle the array.
void shuffle_posScores(PosScore *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        PosScore temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}