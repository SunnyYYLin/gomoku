#include "referee.h"

#include "board.h"
#include <stdio.h>

int is_forbidden(ChessBoard board, Chess newChess) {
    Shape sum_shape = sum_lines(board, newChess);
    if (sum_shape.fives > 0) {
        return -1;
    }
    else if (newChess.color == BLACK && (sum_shape.open_threes+sum_shape.broken_threes > 1 || sum_shape.broken_fours+sum_shape.open_fours > 1 || sum_shape.longs >0)) {
        return 1;
    }
    else {
        return 0;
    }
}

void referee_output(int referee, Chess newChess) {
    if (referee == 0) {
        printf("It's a draw! Game over.\n");
    }
    else if (referee == -1) {
        printf("%s wins! Game over.\n", colorText[newChess.color + 1]);
    }
    else if (referee == 1) {
        printf("BLack had a forbbiden move! Game over.\n");
    }
}