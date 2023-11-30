#include "include.h"
#include <stdio.h>
#include <stdlib.h>

// Global variables
const char* colorText[] = {"White", "Error", "Black"};
ChessBoard board;
Chess newChess;
int referee;

int main() {
    const int MODE = 1;

    RESTART:
    board = empty_board();
    newChess = empty_chess();
    referee = 0;

    system("chcp 65001");

    switch (MODE) {
        case 0:
            for (newChess.turn = 0; newChess.turn <= SIZE*SIZE && referee == 0; newChess.turn++) {
                newChess.color = chess_color(newChess);
                if (newChess.turn == 0) {
                    print_board(board, newChess);
                }
                do {
                    newChess.pos = chess_human_drop(newChess);
                }
                while (!is_valid(board, newChess));
                system("cls");
                board = drop_board(board, newChess);
                print_board(board, newChess);
                referee = is_forbidden(board, newChess);
            }
            break;
        case 1:
            for (newChess.turn = 0; newChess.turn <= SIZE*SIZE && referee == 0; newChess.turn++) {
                newChess.color = chess_color(newChess);
                if (newChess.turn == 0) {
                    system("chcp 65001");
                    print_board(board, newChess);
                }
                newChess.pos = chess_random_drop(board, newChess);
                system("cls");
                board = drop_board(board, newChess);
                print_board(board, newChess);
                referee = is_forbidden(board, newChess);
            }
            break;
        // case 2:
        //     for (newChess.turn = 0; newChess.turn <= SIZE*SIZE && referee == 0; newChess.turn++) {
        //         newChess.color = chess_color(newChess);
        //         if (newChess.turn == 0) {
        //             system("chcp 65001");
        //             print_board(board);
        //         }
        //         switch (color) {
        //             case human_color:
        //                 do {
        //                     newChess.pos = chess_human_drop(newChess);
        //                 }
        //                 while (!is_valid(board, newChess));
        //                 break;
        //             case ai_color:
        //                 do {
        //                     newChess.pos = chess_ai_drop(newChess);
        //                 }
        //                 while (!is_valid(board, newChess));
        //                 break;
        //         }
        //         system("cls");
        //         board = drop_board(board, newChess);
        //         print_board(board, newChess);
        //         referee = is_forbidden(board, newChess);
        //     }
        //     break;
        default:
            printf("Mode Error.");
            break;
    }

    referee_output(referee, newChess);
    // Shape sum_shape = sum_lines(board, newChess);
    // printf("Live threes: %d\n", sum_shape.open_threes + sum_shape.broken_threes);
    // printf("Fours: %d\n", sum_shape.open_fours + sum_shape.broken_fours);
    // printf("Longs: %d\n", sum_shape.longs);
    // printf("Fives: %d\n", sum_shape.fives);

    getchar();
    goto RESTART;

    return 0;
}