#include "referee.h"

#include "symbol.h"
#include "position.h"
#include "line.h"
#include <stdio.h>

// External variables
extern int board[SIZE][SIZE];
extern int turn;
extern Position pos_new;



int exist_forbbiden(ChessColor color, Shape sum_shape) {
    if (sum_shape.live_threes >= 2 || sum_shape.fours >= 2 || sum_shape.longs >= 1) {
        return 1;
    }
    else {
        return 0;
    }
}

int referee_board(ChessColor color) {
    Shape sum_shape = sum_lines(color, pos_new);

    if (color == BLACK && exist_forbbiden(color, sum_shape)){
        printf("Black is forbidden to place here! White wins.\n");
        return -1;
    }
    else if (sum_shape.fives >= 1){
        printf("Game over! %s wins!\n", colorText[color + 1]);
        return 1;
    }

    return 0;
}