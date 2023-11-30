#include "board.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

extern Chess newChess;

Chess empty_chess() {
    Chess empty = {-1, EMPTY, {0,0}};
    return empty;
}

int chess_color(Chess chess) {
    return chess.turn % 2 == 0 ? BLACK : WHITE;
}

Chess make_chess(int color, Position pos) {
    Chess chess = {0, color, {pos.x, pos.y}};
    return chess;
}

Position chess_human_drop(Chess newChess) {
    int x, y;
    char colChar;

    // Ask for the position of the chess
    printf("Round %d is %s's. Drop on the board (example: B4):\n", newChess.turn, colorText[newChess.color + 1]);
    
    // Read the input
    if (scanf(" %c%d", &colChar, &x) != 2) {
        printf("Invalid input format! Please use the format like 'B4'.\n");
    }

    y = toupper(colChar) - 'A';
    x = x - 1;

    Position pos = pos_make(x, y);

    return pos;
}

Position chess_random_drop(ChessBoard board, Chess newChess) {
    int x, y;
    int count = 0;
    Position valid_positions[SIZE*SIZE];

    srand(time(NULL));

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            newChess.pos = pos_make(i, j);
            if (is_valid_silent(board, newChess)) {
                valid_positions[count] = newChess.pos;
                count++;
            }
        }
    }

    int random = rand() % count;

    return valid_positions[random];
}