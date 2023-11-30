#include "include.h"
#include <stdio.h>
#include <stdlib.h>

// 声明外部变量
// Global variables
const char* colorText[] = {"White", "Error", "Black"};
ChessBoard board;
Chess newChess;
int referee = 0;

void shape_print(Shape shape);
void open_threes_test1();
void open_threes_test2();
void broken_threes_test1();
void broken_threes_test2();
void broken_threes_test3();
void open_fours_test1();
void open_fours_test2();
void broken_fours_test1();
void broken_fours_test2();
void broken_fours_test3();
void longs_test1();
void test1();
void test2();
void test3();

int main() {
    system("chcp 65001");
    test4();
    getchar();

    return 0;
}

void shape_print(Shape shape) {
    printf("fives=%d, longs=%d, open_threes=%d, broken_threes=%d, open_fours=%d, broken_fours=%d\n", shape.fives, shape.longs, shape.open_threes, shape.broken_threes, shape.open_fours, shape.broken_fours);
}

void open_threes_test1() {
    ChessBoard board = empty_board();
    Chess black = {-1, BLACK, {7, 7}};

    board.board[2][5] = black;
    board.board[2][6] = black;
    board.board[1][8] = black;
    board.board[0][9] = black;

    Chess newChess = {0,BLACK,{2,7}};
    board = drop_board(board, newChess);

    print_board(board, newChess);
    shape_print(sum_lines(board, newChess));
}

void open_threes_test2() {
    ChessBoard board = empty_board();
    Chess black = {-1, BLACK, {7, 7}};

    board.board[7][5] = black;
    board.board[7][6] = black;
    board.board[6][8] = black;
    board.board[8][6] = black;

    Chess newChess = {0,BLACK,{7,7}};
    board = drop_board(board, newChess);

    print_board(board, newChess);
    shape_print(sum_lines(board, newChess));
}

void broken_threes_test1() {
    ChessBoard board = empty_board();
    Chess black = {-1, BLACK, {7, 7}};

    board.board[7][5] = black;
    board.board[7][6] = black;
    board.board[5][9] = black;
    board.board[8][6] = black;

    Chess newChess = {0,BLACK,{7,7}};
    board = drop_board(board, newChess);

    print_board(board, newChess);
    shape_print(sum_lines(board, newChess));
}

void broken_threes_test2() {
    ChessBoard board = empty_board();
    Chess black = {-1, BLACK, {7, 7}};

    board.board[7][5] = black;
    board.board[7][6] = black;
    board.board[5][9] = black;
    board.board[8][6] = black;
    board.board[6][8] = black;

    Chess newChess = {0,BLACK,{7,7}};
    board = drop_board(board, newChess);

    print_board(board, newChess);
    shape_print(sum_lines(board, newChess));
}

void broken_threes_test3() {
    ChessBoard board = empty_board();
    Chess black = {-1, BLACK, {7, 7}};

    board.board[7][5] = black;
    board.board[7][8] = black;
    board.board[5][9] = black;
    board.board[8][6] = black;

    Chess newChess = {0,BLACK,{7,7}};
    board = drop_board(board, newChess);

    print_board(board, newChess);
    shape_print(sum_lines(board, newChess));
}

void open_fours_test1() {
    ChessBoard board = empty_board();
    Chess black = {-1, BLACK, {7, 7}};

    board.board[7][5] = black;
    board.board[7][6] = black;
    board.board[7][8] = black;
    board.board[6][8] = black;
    board.board[5][9] = black;
    board.board[4][10] = black;

    Chess newChess = {0,BLACK,{7,7}};
    board = drop_board(board, newChess);

    print_board(board, newChess);
    shape_print(sum_lines(board, newChess));
}

void open_fours_test2() {
    ChessBoard board = empty_board();
    Chess black = {-1, BLACK, {7, 7}};
    Chess white = {-1, WHITE, {7, 7}};

    board.board[7][5] = black;
    board.board[7][6] = black;
    board.board[7][8] = black;
    board.board[6][8] = black;
    board.board[5][9] = black;
    board.board[4][10] = black;
    board.board[7][4] = white;

    Chess newChess = {0,BLACK,{7,7}};
    board = drop_board(board, newChess);

    print_board(board, newChess);
    shape_print(sum_lines(board, newChess));
}

void broken_fours_test1() {
    ChessBoard board = empty_board();
    Chess black = {-1, BLACK, {7, 7}};
    Chess white = {-1, WHITE, {7, 7}};

    board.board[7][3] = black;
    board.board[7][4] = black;
    board.board[7][5] = black;
    board.board[8][6] = black;
    board.board[5][9] = black;
    board.board[4][10] = black;

    Chess newChess = {0,BLACK,{7,7}};
    board = drop_board(board, newChess);

    print_board(board, newChess);
    shape_print(sum_lines(board, newChess));
}

void broken_fours_test2() {
    ChessBoard board = empty_board();
    Chess black = {-1, BLACK, {7, 7}};
    Chess white = {-1, WHITE, {7, 7}};

    board.board[7][3] = black;
    board.board[7][4] = black;
    board.board[7][5] = black;
    board.board[8][6] = black;
    board.board[5][9] = black;
    board.board[4][10] = black;
    board.board[7][6] = white;
    board.board[9][5] = white;

    Chess newChess = {0,BLACK,{7,7}};
    board = drop_board(board, newChess);

    print_board(board, newChess);
    shape_print(sum_lines(board, newChess));
}

void broken_fours_test3() {
    ChessBoard board = empty_board();
    Chess black = {-1, BLACK, {7, 7}};
    Chess white = {-1, WHITE, {7, 7}};

    board.board[7][3] = black;
    board.board[7][4] = black;
    board.board[7][6] = black;
    board.board[7][9] = black;
    board.board[7][10] = black;

    Chess newChess = {0,BLACK,{7,7}};
    board = drop_board(board, newChess);

    print_board(board, newChess);
    shape_print(sum_lines(board, newChess));
}

void longs_test1() {
    ChessBoard board = empty_board();
    Chess black = {-1, BLACK, {7, 7}};
    Chess white = {-1, WHITE, {7, 7}};

    board.board[7][3] = black;
    board.board[7][4] = black;
    board.board[7][5] = black;
    board.board[7][6] = black;
    board.board[7][8] = black;
    board.board[7][9] = black;
    board.board[7][10] = white;

    Chess newChess = {0,BLACK,{7,7}};
    board = drop_board(board, newChess);

    print_board(board, newChess);
    shape_print(sum_lines(board, newChess));
}

void fives_test1() {
    ChessBoard board = empty_board();
    Chess black = {-1, BLACK, {7, 7}};
    Chess white = {-1, WHITE, {7, 7}};

    board.board[7][3] = black;
    board.board[7][4] = black;
    board.board[7][5] = black;
    board.board[7][6] = black;

    Chess newChess = {0,BLACK,{7,7}};
    board = drop_board(board, newChess);

    print_board(board, newChess);

    shape_print(sum_lines(board, newChess));
}

void test1() {
    ChessBoard board = empty_board();
    Chess black = {-1, BLACK, {7, 7}};
    Chess white = {-1, WHITE, {7, 7}};

    board.board[7][2] = black;
    board.board[7][3] = black;
    board.board[7][4] = black;
    board.board[7][5] = black;

    Chess newChess = {0,BLACK,{7,7}};
    board = drop_board(board, newChess);

    print_board(board, newChess);

    shape_print(sum_lines(board, newChess));
}

void test2() {
    ChessBoard board = empty_board();
    Chess black = {-1, BLACK, {7, 7}};
    Chess white = {-1, WHITE, {7, 7}};

    board.board[7][4] = black;
    board.board[7][5] = black;
    board.board[7][6] = black;

    Chess newChess = {0,BLACK,{7,7}};
    board = drop_board(board, newChess);

    print_board(board, newChess);

    shape_print(sum_lines(board, newChess));
}

void test3() {
    ChessBoard board = empty_board();
    Chess black = {-1, BLACK, {7, 7}};
    Chess white = {-1, WHITE, {7, 7}};

    board.board[7][5] = black;
    board.board[7][6] = black;
    board.board[7][9] = black;
    board.board[7][10] = black;
    board.board[8][7] = black;
    board.board[9][7] = black;
    board.board[10][7] = black;

    Chess newChess = {0,BLACK,{7,7}};
    board = drop_board(board, newChess);

    print_board(board, newChess);

    shape_print(sum_lines(board, newChess));
}

void test4() {
        ChessBoard board = empty_board();
    Chess black = {-1, BLACK, {7, 7}};
    Chess white = {-1, WHITE, {7, 7}};

    board.board[0][0] = black;
    board.board[0][1] = black;
    board.board[1][2] = black;
    board.board[2][2] = black;


    Chess newChess = {0,BLACK,{0,2}};
    board = drop_board(board, newChess);

    print_board(board, newChess);

    shape_print(sum_lines(board, newChess));
}