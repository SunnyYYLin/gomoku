#include "include.h"
#include <stdio.h>
#include <stdlib.h>

// Global variables
const char* colorText[] = {"White", "Error", "Black"};
ChessBoard board;
Chess newChess;
int referee = 0;

int main() {
    const int MODE = 1;

    board = empty_board();
    newChess = empty_chess();

    switch (MODE) {
        case 0:
            for (newChess.turn = 0; newChess.turn <= SIZE*SIZE && referee == 0; newChess.turn++) {
                newChess.color = chess_color(newChess);
                if (newChess.turn == 0) {
                    system("chcp 65001");
                    print_board(board);
                }
                do {
                    newChess.pos = chess_human_drop(newChess);
                }
                while (!is_valid(board, newChess));
                system("cls");
                board = update_board(board, newChess);
                print_board(board);
                referee = referee_board(board, newChess);
            }
            break;
        case 1:
            for (newChess.turn = 0; newChess.turn <= SIZE*SIZE && referee == 0; newChess.turn++) {
                newChess.color = chess_color(newChess);
                if (newChess.turn == 0) {
                    system("chcp 65001");
                    print_board(board);
                }
                newChess.pos = chess_random_drop(board, newChess);
                system("cls");
                board = update_board(board, newChess);
                print_board(board);
                referee = referee_board(board, newChess);
            }
            break;
        default:
            printf("Mode Error.");
            break;
    }

    referee_output(referee, newChess);
    Shape sum_shape = sum_lines(board, newChess);
    printf("Live threes: %d\n", sum_shape.live_threes);
    printf("Fours: %d\n", sum_shape.fours);
    printf("Longs: %d\n", sum_shape.longs);
    printf("Fives: %d\n", sum_shape.fives);

    getchar();
    getchar();

    return 0;
}