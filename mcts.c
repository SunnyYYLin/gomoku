#include "mcts.h"

#include "chess.h"
#include "position.h"
#include "ai.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

const double C = 1.41;

extern const genann *ann;

int is_root(MCTS_Node *node) {
    return node->parent == NULL;
}

int is_leaf(MCTS_Node *node) {
    return node->num_children == 0;
}

MCTS_Node *node_create(MCTS_Node *parent, Position pos) {
    MCTS_Node *node = malloc(sizeof(MCTS_Node));
    if (node == NULL) {
        // 处理内存分配失败
        return NULL;
    }

    if (parent == NULL) {
        node->board = empty_board();
        node->chess.color = BLACK;
        node->chess.turn = 0;
        node->chess.pos = pos_make(SIZE/2, SIZE/2);
    }
    else {
        memcpy(node->board, parent->board, sizeof(node->board));
        node->chess.color = -parent->chess.color;
        node->chess.turn = parent->chess.turn + 1;
        node->chess.pos = pos;
    }
    node->board[pos.x][pos.y] = node->chess;

    node->parent = parent;
    node->visit_count = 0;
    node->children = NULL;
    node->num_children = 0;
    node->win_rate = ai_win_rate(ann, node->board);

    if (parent != NULL) {
        struct MCTS_Node **new_children = realloc(parent->children, (parent->num_children + 1) * sizeof(MCTS_Node*));
        if (new_children == NULL) {
            free(node);
            return NULL;
        }
        parent->children = new_children;
        parent->children[parent->num_children] = node;
        parent->num_children++;
    }

    return node;
}

int num_valid_moves(MCTS_Node *node) {
    int possible_moves = 0;
    Chess board[SIZE][SIZE] = node->board;
    // 首先，计算可能的移动数量
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (is_valid(node->board, pos_make(i, j))) {
                ++possible_moves;
            }
        }
    }
}

void mtcs_expand(MCTS_Node *node) {
    int possible_moves = num_valid_moves(node);

    // 为每个可能的移动创建一个子节点
    node->children = malloc(possible_moves * sizeof(MCTS_Node));
    if (node->children == NULL) {
        exit(1);  // 处理内存分配失败
    }
    node->num_children = possible_moves;
    int index = 0;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (is_valid(pos_make(i, j))) {
                node->children[index] = node_create(node, pos_make(i, j), index;
                index ++;
            }
        }
    }
}

MCTS_Node* select(MCTS_Node* node, double c_puct) {
    while (!is_leaf(node)) {
        MCTS_Node* best_child = NULL;
        double max_ucb = -INFINITY;

        for (int i = 0; i < node->num_children; ++i) {
            MCTS_Node* child = node->children[i];
            double ucb_value = child->win_rate + c_puct * sqrt(log(node->visit_count) / (1 + child->visit_count));

            if (ucb_value > max_ucb) {
                max_ucb = ucb_value;
                best_child = child;
            }
        }

        node = best_child;
    }
    return node;
}

void update(MCTS_Node* node, double leaf_value) {
    while (node != NULL) {
        node->visit_count++;
        node->win_rate += (leaf_value - node->win_rate) / node->visit_count;
        node = node->parent;
    }
}

int simulate(MCTS_Node* node) {
    MCTS_Node* current = copyNode(node);

    while (referee(current->board) != 0) {
        current = makeRandomMove(current);
    }

    int result = referee(current->board);
    freeNode(current);
    return result;
}