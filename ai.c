#include "ai.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

// Creates a deep copy of the game board.
int** copy_board(int** board) {
    int** new_board = malloc(SIZE * sizeof(int*));
    if (new_board == NULL) {
        // Memory allocation failed
        return NULL;
    }

    for (int i = 0; i < SIZE; i++) {
        new_board[i] = malloc(SIZE * sizeof(int));
        if (new_board[i] == NULL) {
            // Memory allocation failed, free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(new_board[j]);
            }
            free(new_board);
            return NULL;
        }
        memcpy(new_board[i], board[i], SIZE * sizeof(int));
    }
    return new_board;
}

// Frees memory allocated for the game board.
void free_board(int** board) {
    for (int i = 0; i < SIZE; i++) {
        free(board[i]);
    }
    free(board);
}

// Calculates the score for a given shape.
int score_shape(Shape shape, int color, AI ai) {
    int score = 0;
    score += shape.fives * ai.score_five;
    score += shape.open_fours * ai.score_open_four;
    score += shape.broken_fours * ai.score_broken_four;
    score += shape.open_threes * ai.score_open_three;
    score += shape.broken_threes * ai.score_broken_three;
    score += shape.open_twos * ai.score_open_two;
    score += shape.broken_twos * ai.score_broken_two;
    return score;
}

// Prints scores for each valid position on the board.
void print_scores(Position* valid_pos, int* score, int valid_count) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Position pos = pos_make(i, j);  // Assume pos_make creates a position
            int found = 0;
            for (int k = 0; k < valid_count; k++) {
                if (is_pos_equal(pos, valid_pos[k])) {  // Assume pos_equal compares two positions for equality
                    printf("%d ", score[k]);
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

// Evaluates scores for all valid moves.
int* evaluate(int** board, int color, Position* valid_pos, int valid_count, AI ai) {
    Position pos;
    int* scores = (int*)malloc(sizeof(int) * valid_count);

    for (int k=0; k<valid_count; k++) {
        pos = valid_pos[k];
        scores[k] = evaluate_move(board, pos, color, ai) + evaluate_move(board, pos, -color, ai);
    }

    return scores;
}

// Evaluates the score of a single move.
int evaluate_move(int** board, Position pos, int color, AI ai) {
    int** board_copy = copy_board(board);
    board_copy[pos.x][pos.y] = color;

    int score = 0;
    Shape* shapes = enroll_lines(board_copy, pos, color);
    Shape shape = sum_lines(shapes);
    if (is_win(shape)) {
        score = 10*ai.score_five;
    }
    else if (is_forbidden(shape, color)) {
        score = -10*ai.score_five;
    }
    else {
        score = score_shape(shape, color, ai);
    }

    free_board(board_copy);
    return score;
}

// Determines the best position for the AI to drop a piece.
Position ai_drop(int** board, int color, AI ai) {
    int score;
    int max_score = -1;
    int valid_count;
    int best_moves[SIZE*SIZE];
    int num_best_moves = 0;

    Position* valid_pos = valid_positions(board, color, &valid_count);
    int* scores = evaluate(board, color, valid_pos, valid_count, ai);
    // print_scores(valid_pos, scores, valid_count);
    for (int l=0; l<valid_count; l++) {
        score = scores[l];
        if (score > max_score) {
            max_score = score;
            num_best_moves = 0;
            best_moves[num_best_moves] = l;
        }
        else if (score == max_score) {
            num_best_moves++;
            best_moves[num_best_moves] = l;
        }
    }
    
    Position pos = valid_pos[best_moves[rand() % (num_best_moves+1)]];
    free(scores);
    free(valid_pos);
    return pos;
}

void ai_train();