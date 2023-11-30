#include "board.h"

#include <stdio.h>
#include <ctype.h>

extern Chess newChess;

ChessBoard empty_board() {
    ChessBoard board;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board.board[i][j] = empty_chess();
        }
    }
    return board;
}

// ChessBoard board_clear_visit (ChessBoard board){
//     for (int i = 0; i < SIZE; i++) {
//         for (int j = 0; j < SIZE; j++) {
//             board.board[i][j].has_been_visited = 0;
//         }
//     }
//     return board;
// }

// Display the current state of the board and Trun NEW to DEFAULT
void print_board(ChessBoard board, int turn) {
    printf("Round %d\n", turn);

    // Print column headers
    printf("  ");
    for (int i = 0; i < SIZE; i++) {
        printf("%c ", 'A' + i);
    }
    printf("\n");

    // Print each row of the board
    for (int i = 0; i < SIZE; i++) {
        printf("%2d", i + 1); // Print row number
        for (int j = 0; j < SIZE; j++) {
            // Print the appropriate character based on the board's state
            switch (board.board[i][j].color) {
                case EMPTY:
                    // Special characters for the board's edges and corners
                    if (i == 0 && j == 0) printf("┌");
                    else if (i == 0 && j == SIZE-1) printf("┐");
                    else if (i == SIZE-1 && j == 0) printf("└");
                    else if (i == SIZE-1 && j == SIZE-1) printf("┘");
                    else if (i == 0) printf("┬");
                    else if (i == SIZE-1) printf("┴");
                    else if (j == 0) printf("├");
                    else if (j == SIZE-1) printf("┤");
                    else printf("┼");
                    break;
                case BLACK:
                    if (board.board[i][j].turn == turn) {
                        printf("▲");
                    }
                    else {
                        printf("●");
                    }
                    break;
                case WHITE:
                    if (board.board[i][j].turn == turn) {
                        printf("△");
                    }
                    else {
                        printf("○");
                    }
                    break;
            }
        }
        printf("\n");
    }
}

ChessBoard drop_board(ChessBoard board, Chess newChess) {
    board.board[newChess.pos.x][newChess.pos.y] = newChess;
    return board;
}

ChessBoard undo_board(ChessBoard board, Chess newChess) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++){
            if (board.board[i][j].turn == newChess.turn - 1) {
                board.board[i][j] = empty_chess();
            }
        }
    }

    return board;
}