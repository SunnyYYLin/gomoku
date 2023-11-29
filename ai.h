#ifndef AI_H
#define AI_H

#include "genann.h"

genann *init_neural_network();
double *board_to_input(Chess board[SIZE][SIZE]);
void train_neural_network(genann *ann, double *input, double *output);
double ai_win_rate(const genann *ann, Chess board[SIZE][SIZE]);

#endif