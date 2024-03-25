#ifndef MTCS_H
#define MTCS_H

#include "board.h"
extern int **board;

// MCTS Node: the identifier of parameters is the same as in the article of AlphaGo Zero
typedef struct Node {
    Position pos;
    int color; // color of the player who made the move to reach this node
    double P; // prior probability of choosing this node
    double Q; // average value of all visits to this node
    double W; // total value of all visits to this node
    int N; // number of times this node has been visited
    double U; // upper confidence bound for this node
    struct Node* parent;
    struct Node** children;
    int numChildren;
} Node;

// Create a new MCTSNode
Node* node_create(Position pos, double priorProbability, Node* parent);

// Free the memory of a MCTSNode
void node_free(Node* node);

#endif // MTCS_H