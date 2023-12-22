#include "include.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Global variables
const char* colorText[] = {"White", "Error", "Black"};
ChessBoard board;
Chess newChess;
Player player1, player2;
int referee;
int quit;

int main() {
    const int MODE = 1;

    // Choose player set
    while (!is_player_set_valid(player1, player2)) {
        player1 = set_player();
        player2 = set_player();
    }

    // Initialize
    RESTART:

    switch (MODE) {
        case 0:
            for (newChess.turn = 0; newChess.turn <= SIZE*SIZE && referee == 0; newChess.turn++) {
                if (newChess.turn == 0) {
                    UNDO:
                    system("cls");
                    print_board(board, newChess.turn - 1);
                }
                newChess.color = chess_color(newChess);
                do {
                    newChess.pos = chess_human_drop(newChess);
                    if (newChess.pos.y == 'X'-'A' && newChess.pos.x == 0) {
                        board = undo_board(board, newChess);
                        newChess.turn--;
                        goto UNDO;
                    }
                }
                while (!is_valid(board, newChess));
                system("cls");
                board = drop_board(board, newChess);
                print_board(board, newChess.turn);
                referee = is_forbidden(board, newChess);
            }
            break;
        case 1:
            srand(time(NULL));
            for (newChess.turn = 0; newChess.turn <= SIZE*SIZE && referee == 0; newChess.turn++) {
                newChess.color = chess_color(newChess);
                if (newChess.turn == 0) {
                    system("cls");
                    print_board(board, newChess.turn - 1);
                }
                newChess.pos = chess_random_drop(board, newChess);
                system("cls");
                board = drop_board(board, newChess);
                print_board(board, newChess.turn);
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

    // If quit?
    printf("Press any key to continue, or press 'q' to quit.\n");
    quit = getchar();
    if (quit == 'q' || quit == 'Q') {
        return 0;
    }
    else {
        printf("Invalid input! Please try again.\n");
        goto RESTART;
    }
}