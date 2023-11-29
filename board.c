#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "symbol.h"
#include "position.h"

// External variables
extern int board[SIZE][SIZE];
extern int turn;
extern Position pos_new;

// Initialize the board with all positions empty
void init_board() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
}

// Display the current state of the board
void print_board() {
    printf("Round %d\n", turn);

    // Print column headers
    printf("  ");
    for (int i = 0; i < SIZE; i++) {
        printf("%c", 'A' + i);
    }
    printf("\n");

    // Print each row of the board
    for (int i = 0; i < SIZE; i++) {
        printf("%2d", i + 1); // Print row number
        for (int j = 0; j < SIZE; j++) {
            // Print the appropriate character based on the board's state
            switch (board[i][j]) {
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
                    printf("●");
                    break;
                case BLACK_NEW:
                    printf("▲");
                    break;
                case WHITE:
                    printf("○");
                    break;
                case WHITE_NEW:
                    printf("△");
                    break;
            }
        }
        printf("\n");
    }
}

// Put a chess on the board 
Position drop_board(int color) {
    int x, y;
    char colChar;

    // Ask for the position of the chess
    printf("Round %d is %s's. Drop on the board (example: B4):\n", turn + 1, colorText[color + 1]);
    
    // Read the input
    if (scanf(" %c%d", &colChar, &x) != 2) {
        printf("Invalid input format! Please use the format like 'B4'.\n");
    }

    y = toupper(colChar) - 'A';
    x = x - 1; // Convert the position to the index of the array

    return pos_make(x, y);
}

// Update the board after each move
void update_board(ChessColor color) {
    if (turn != 0) {
        board[pos_new.x][pos_new.y] = colorNew[color + 1];
        print_board();
        board[pos_new.x][pos_new.y] = color;
    } 
    else {
        print_board();
    }

    // Keep prompting the player until a valid move is made
    do {
        pos_new = drop_board(color);
    }
    while (!is_valid(pos_new));

    system("cls"); // Clear the console
}