#include "MCTS.h"
#include <stddef.h>
#include <math.h>

Node* node_create(Position pos, double priorProbability, Node* parent) {
    Node* node = malloc(sizeof(Node));

    node->pos = pos;
    node->color = -parent->color;
    node->P = priorProbability;
    node->Q = 0;
    node->W = 0;
    node->N = 0;
    node->U = 65535;
    node->parent = parent;
    node->children = NULL;
    node->numChildren = 0;

    return node;
}

void node_free(Node* node) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < node->numChildren; i++) {
        node_free(node->children[i]);
    }
    free(node->children);
    free(node);
}

int is_leaf(Node* node) {
    return node->numChildren == 0;
}

Node* node_select(Node* node, double cpuct) {
    while (!is_leaf(node)) {
        Node* bestChild = NULL;
        double bestValue = -INFINITY;

        for (int i = 0; i < node->numChildren; i++) {
            Node* child = node->children[i];
            double ucbValue = get_UCB(child, node, cpuct);
            if (ucbValue > bestValue) {
                bestChild = child;
                bestValue = ucbValue;
            }
        }

        node = bestChild;
    }
    return node;
}

void node_backup(Node* node, double value) {
    if (node == NULL) {
        return;
    }

    // Increment the visit count
    node->N += 1;
    // Update the total value (W)
    node->W += value;
    // Recalculate the average value (Q)
    node->Q = node->W / node->N;

    // Recursively call backup on the parent node
    node_backup(node->parent, value);
}

void node_expand(Node* node) {
    int numActions = 0;
    int** board_sim = node_get_board(node);
    Position* actions = get_empty_positions(board_sim, &numActions);
    int* policies = malloc(numActions * sizeof(int));

    // Transform and evaluate the node state using the neural network
    int output_p[SIZE*SIZE];
    double value = nn_evaluate(board_sim, output_p);
    for (int a = 0; a < numActions; a++) {
        policies[a] = output_p[SIZE*actions[a].x + actions[a].y];
    }

    // Allocate memory for children
    node->children = malloc(numActions * sizeof(Node*));
    node->numChildren = numActions;

    // Initialize each child/action edge
    for (int a = 0; a < numActions; a++) {
        node->children[a] = node_create(actions[a], policies[a], node);
    }

    // Back up the value in the tree
    node_backup(node, value);
}

Node* node_play(Node* root, double temperature) {
    double total = 0;
    double* probabilities = malloc(root->numChildren * sizeof(double));

    // Calculate the exponentiated visit counts and total sum
    for (int i = 0; i < root->numChildren; i++) {
        probabilities[i] = pow(root->children[i]->N, 1 / temperature);
        total += probabilities[i];
    }

    // Normalize probabilities
    for (int i = 0; i < root->numChildren; i++) {
        probabilities[i] /= total;
    }

    // Select a move based on normalized probabilities
    int selectedAction = select_action(probabilities, root->numChildren);

    // Return the selected child node
    return root->children[selectedAction];
}

int** node_get_board(Node* node) {
    int** board_sim = copy_board(board);

    Node* parent = node;
    while (parent != NULL) {
        board_sim[parent->pos.x][parent->pos.y] = parent->color;
        parent = parent->parent;
    }

    return board_sim;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      

/*-----------------------------------------------------------------------------------------------------------*/

double get_UCB(Node* child, Node* parent, double cpuct) {
    double qValue = child->Q;
    double pValue = child->P;
    int totalVisits = parent->N;
    int childVisits = child->N;

    return qValue + cpuct * pValue * sqrt(totalVisits) / (1 + childVisits);
}