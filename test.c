#include "symbol.h"
#include "board.h"
#include "referee.h"
#include <stdio.h>

// 声明外部变量
extern int board[SIZE][SIZE];
extern Position pos_new;

void test_forbidden_moves() {
    // 测试双三禁手
    init_board();
    board[7][6] = BLACK;
    board[7][7] = BLACK; 
    board[7][8] = BLACK; 
    board[6][6] = BLACK;
    board[8][6] = BLACK;
    pos_new.x = 7; pos_new.y = 6;
    printf("Testing Double Three Forbidden Move...\n");
    printf("Referee result: %d\n\n\n\n", referee_board(BLACK));

    // 测试双四禁手
    init_board();
    board[7][7] = BLACK; 
    board[7][8] = BLACK; 
    board[7][6] = BLACK; 
    board[7][5] = BLACK; 
    board[6][7] = BLACK;
    board[8][7] = BLACK;
    board[9][7] = BLACK;
    pos_new.x = 7; pos_new.y = 7;
    printf("Testing Double Four Forbidden Move...\n");
    printf("Referee result: %d\n\n\n\n", referee_board(BLACK));

    // 测试长连禁手
    init_board();
    for (int i = 5; i <= 10; i++) {
        board[7][i] = BLACK;
    }
    pos_new.x = 7; pos_new.y = 10;
    printf("Testing Long Line Forbidden Move...\n");
    printf("Referee result: %d\n\n\n\n", referee_board(BLACK));
}

int main() {
    test_forbidden_moves();
    getchar();
    return 0;
}
