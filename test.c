#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "shape.h"

void test1(int** board) {
    board[7][7] = BLACK;
    board[7][8] = BLACK;
    board[7][9] = BLACK;
}

void test2(int** board) {
    board[7][7] = BLACK;
    board[7][8] = BLACK;
    board[7][11] = BLACK;
}

void test3(int** board) {
    board[7][7] = BLACK;
    board[7][8] = BLACK;
    board[7][9] = BLACK;
    board[7][10] = BLACK;
    board[7][11] = BLACK;
}

void test4(int** board) {
    board[7][7] = BLACK;
    board[7][8] = BLACK;
}

void test5(int** board) {
    board[7][7] = WHITE;
    board[7][8] = BLACK;
    board[7][9] = BLACK;
    board[7][10] = BLACK;
    board[7][11] = BLACK;
}

void test6(int** board) {
    board[7][7] = BLACK;
    board[6][8] = BLACK;
    board[8][6] = BLACK;
    board[9][5] = BLACK;
    board[8][7] = WHITE;
    board[11][3] = WHITE;
    // board[8][8] = BLACK;
}

void test7(int** board) {
    board[7][7] = BLACK;
    board[7][8] = BLACK;
    board[7][6] = BLACK;
    board[7][3] = WHITE;
    board[7][11] = WHITE;
}

int turn = 2;

int main() {
    system("chcp 65001");
    int color = BLACK;
    int** board;
    init_board(&board);
    test7(board);
    Position pos = {7, 7};
    print_board(board, pos, 2);
    enroll_lines(board, pos, color);
    Shape* shapes = enroll_lines(board, pos, color);
    Shape sum_shape = sum_lines(shapes);
    print_shape(sum_shape);
    getchar();
    return 0;
} 