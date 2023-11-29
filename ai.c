#include "ai.h"

#include <stdlib.h>
#include <time.h>
#include "genann.h"
#include "chess.h"

extern Chess newChess;

genann *init_neural_network() {
    int input_nodes = SIZE * SIZE;
    int hidden_nodes1 = 64;
    int hidden_nodes2 = 64;
    int output_nodes = 1;

    genann *ann = genann_init(input_nodes, 2, hidden_nodes1, hidden_nodes2, output_nodes);

    return ann;
}

double *board_to_input(Chess board[SIZE][SIZE]) {
    double *input = (double *)malloc(SIZE * SIZE * sizeof(double));
    
    if (input == NULL) {
        printf("Error: malloc failed in board_to_input()\n");
        exit(1);
    }

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            input[i * SIZE + j] = (double)board[i][j].color;
        }
    }

    return input;
}

void train_neural_network(genann *ann, double *input, double *output) {
    genann_train(ann, input, output, 0.1);
}

double ai_win_rate(const genann *ann, Chess board[SIZE][SIZE]) {
    double *input = board_to_input(board[SIZE][SIZE]);
    const double *output = genann_run(ann, input);
    double result = *output;
    free(input);
    return result;
}