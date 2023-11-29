#include "referee.h"

#include "board.h"
#include <stdio.h>

int is_forbbiden(ChessBoard board, Chess newChess, int depth) {
    if (depth <= 1) {
        board.board[newChess.pos.x][newChess.pos.y] = newChess;
    
        Shape sum_shape = sum_lines(board, newChess);
        if ((sum_shape.live_threes >= 2 || sum_shape.fours >= 2 || sum_shape.longs >= 1) && newChess.color == BLACK) {
            return 1;
        }
        else if (sum_shape.fives >= 1){
            return -1;
        }
        else {
            return 0;
        }
    }
    else {
        return !is_empty(board, newChess.pos);
    }
}

void referee_output(int referee, Chess newChess) {
    if (referee == 0) {
        printf("It's a draw! Game over.\n");
    }
    else if (referee == 1) {
        printf("%s wins! Game over.\n", colorText[newChess.color + 2]);
    }
    else if (referee == -1) {
        printf("BLack had a forbbiden move! Game over.\n");
    }
}