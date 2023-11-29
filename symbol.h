#ifndef SYMBOL_H
#define SYMBOL_H

// Define the possible states of each position on the board
typedef enum ChessColor {
    EMPTY = 0,
    BLACK = 1,
    BLACK_NEW = 2,
    WHITE = -1,
    WHITE_NEW = -2
} ChessColor;

#define SIZE 15 // The size of the board
#define BORDER -3 // The border of the board

extern const char* colorText[3]; // The name of each state
extern const ChessColor colorNew[3]; // The new state of each state

#endif // SYMBOL_H