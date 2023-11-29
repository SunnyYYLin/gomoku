#include "mcts.h"

#include "symbol.h"
#include "position.h"
#include <math.h>
#include <stdlib.h>

const double C = 1.41;

MCTS_Node *create_node(int board[SIZE][SIZE], MCTS_Node *parent) {
    MCTS_Node *node = malloc(sizeof(MCTS_Node));
    memcpy(node->board, board, sizeof(node->board));
    node->parent = parent;
    node->visit_count = 0;
    node->win_score = 0.0;
    node->children = NULL;
    node->num_children = 0;
    node->player = parent == NULL ? 1 : -parent->player;  // 切换玩家
    return node;
}

MCTS_Node *select_promising_node(MCTS_Node *root) {
    MCTS_Node *current_node = root;

    while (current_node->num_children != 0) {
        MCTS_Node *best_node = NULL;
        double max_ucb = -INFINITY;

        for (int i = 0; i < current_node->num_children; i++) {
            MCTS_Node *child = &current_node->children[i];
            double ucb_value = (child->win_score / (double) child->visit_count) +
                               C * sqrt(log((double) current_node->visit_count) / (double) child->visit_count);

            if (ucb_value > max_ucb) {
                max_ucb = ucb_value;
                best_node = child;
            }
        }

        current_node = best_node;
    }

    return current_node;
}

void expand_node(MCTS_Node *node) {
    int possible_moves = 0;

    // 首先，计算可能的移动数量
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (node->board[i][j] == EMPTY) {  // 假设 EMPTY 定义为没有棋子的状态
                ++possible_moves;
            }
        }
    }

    // 为每个可能的移动创建一个子节点
    node->children = malloc(possible_moves * sizeof(MCTS_Node));
    node->num_children = possible_moves;
    int move_index = 0;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (node->board[i][j] == EMPTY) {
                MCTS_Node *new_node = &node->children[move_index++];
                memcpy(new_node->board, node->board, sizeof(node->board));  // 复制棋盘状态
                new_node->board[i][j] = node->player;  // 假设 player 用 1 和 -1 表示不同玩家
                new_node->parent = node;
                new_node->visit_count = 0;
                new_node->win_score = 0.0;
                new_node->num_children = 0;
                new_node->children = NULL;
                new_node->player = -node->player;  // 切换到对方玩家
            }
        }
    }
}

int simulate_random_playout(MCTS_Node *node) {
    int temp_board[SIZE][SIZE];
    memcpy(temp_board, node->board, sizeof(temp_board));
    int current_player = node->player;

    while (1) {
        // 找出所有可能的移动
        Position possible_moves[SIZE * SIZE];
        int num_possible_moves = 0;
        for (int x = 0; x < SIZE; x++) {
            for (int y = 0; y < SIZE; y++) {
                Position pos = pos_make(x, y);
                if (is_empty(pos)) {
                    possible_moves[num_possible_moves++] = pos;
                }
            }
        }

        // 检查是否还有可行的移动或游戏是否结束
        if (num_possible_moves == 0 || referee_board(current_player, temp_board)) {
            break;
        }

        // 随机选择一个移动
        int move_index = rand() % num_possible_moves;
        Position selected_move = possible_moves[move_index];
        temp_board[selected_move.x][selected_move.y] = current_player;

        // 切换玩家
        current_player = -current_player;
    }

    // 判断游戏结果
    int result = referee_board(current_player, temp_board);
    return result;  // 返回游戏结果，可能需要根据您的规则调整
}

void backpropagation(MCTS_Node *node, int player) {
    MCTS_Node *temp_node = node;
    while (temp_node != NULL) {
        temp_node->visit_count++;
        if (temp_node->player == player) {
            temp_node->win_score += 1;
        }
        temp_node = temp_node->parent;
    }
}

Position position_of(MCTS_Node *node) {
    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
            if (node->board[x][y] != node->parent->board[x][y]) {
                return pos_make(x, y);
            }
        }
    }
    // 这种情况理论上不应该发生，表示没有找到不同之处
    return pos_make(-1, -1);  
}

Position find_best_move(MCTS_Node *root) {
    int max_visits = -1;
    Position best_move;
    for (int i = 0; i < root->num_children; i++) {
        if (root->children[i].visit_count > max_visits) {
            max_visits = root->children[i].visit_count;
            best_move = position_of(&root->children[i]);  // 需要实现这个函数
        }
    }
    return best_move;
}

Position mcts_find_best_move(int board[SIZE][SIZE], int player) {
    MCTS_Node *root = create_node(board, NULL);

    int number_of_iterations = 100;  // 可以根据需要调整迭代次数
    for (int i = 0; i < number_of_iterations; ++i) {
        MCTS_Node *promising_node = select_promising_node(root);
        expand_node(promising_node);
        MCTS_Node *node_to_explore = promising_node;
        if (promising_node->num_children > 0) {
            node_to_explore = &promising_node->children[0];  // 或选择一个随机的子节点
        }
        int playout_result = simulate_random_playout(node_to_explore);
        backpropagation(node_to_explore, playout_result);
    }

    // 选择最佳移动
    Position best_move = find_best_move(root);
    free_mcts_tree(root);  // 清理 MCTS 树
    return best_move;
}

void free_mcts_tree(MCTS_Node *root) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < root->num_children; i++) {
        free_mcts_tree(&root->children[i]);
    }
    free(root->children);
    free(root);
}
