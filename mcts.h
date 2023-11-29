#ifndef MCTS_H
#define MCTS_H

#include "chess.h"
#include "board.h"
#include "position.h"

typedef struct MCTS_Node {
    ChessBoard board;
    Chess chess;
    struct MCTS_Node *parent;
    struct MCTS_Node **children;
    int num_children;
    double win_rate;
    int visit_count;
} MCTS_Node;

// MCTS 相关函数
MCTS_Node *create_node(int board[SIZE][SIZE], MCTS_Node *parent);
void expand_node(MCTS_Node *node);
MCTS_Node *select_promising_node(MCTS_Node *root);
int simulate_random_playout(MCTS_Node *node);
void backpropagation(MCTS_Node *node, int player);
Position mcts_find_best_move(int board[SIZE][SIZE], int player);
Position find_best_move(MCTS_Node *root);
Position position_of(MCTS_Node *node);
void free_mcts_tree(MCTS_Node *root);

#endif