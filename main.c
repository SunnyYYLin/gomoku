#include "symbol.h"
#include "board.h"
#include "referee.h"
#include "position.h"
#include <stdio.h>

// Global variables
int board[SIZE][SIZE];
int turn;
Position pos_new;

const char* colorText[] = {"White", "Error", "Black"};
const ChessColor colorNew[] = {WHITE_NEW, EMPTY, BLACK_NEW};

// Determine the current player's color based on the turn number
ChessColor getCurrentPlayerColor() {
    return (turn % 2 == 0) ? WHITE : BLACK;
}

int main() {
    int color;

    init_board();

    // Loop through each turn until the board is full
    for (turn = 0; turn <= SIZE*SIZE; turn++) {
        color = getCurrentPlayerColor();
        update_board(color);
        if (referee_board(-color) != 0) break;
    }

    if (referee_board(-color) == 0) {
        printf("It's a draw! Game over.\n");
    }

    getchar();
    getchar();

    return 0;
}