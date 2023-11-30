#include "board.h"

#include <stdio.h>
#include <ctype.h>

Directions directions = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

int is_empty (ChessBoard board, Position pos) {
    int x = pos.x;
    int y = pos.y;

    if (is_in_board(pos)&&board.board[x][y].color == EMPTY) {
        return 1;
    }

    return 0;
}

int is_color (ChessBoard board, Position pos, int color) {
    int x = pos.x;
    int y = pos.y;

    if (is_in_board(pos)&&board.board[x][y].color == color) {
        return 1;
    }

    return 0;
}

int is_center (Position pos) {
    int x = pos.x;
    int y = pos.y;

    if (x == SIZE/2 && y == SIZE/2) {
        return 1;
    }

    return 0;
}

int is_in_board (Position pos) {
    int x = pos.x;
    int y = pos.y;

    if (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
        return 1;
    }

    return 0;
}

int is_on_edge (Position pos) {
    int x = pos.x;
    int y = pos.y;

    if (x == 0 || x == SIZE-1 || y == 0 || y == SIZE-1) {
        return 1;
    }

    return 0;
}

int is_on_corner (Position pos) {
    int x = pos.x;
    int y = pos.y;

    if ((x == 0 || x == SIZE-1) && (y == 0 || y == SIZE-1)) {
        return 1;
    }

    return 0;
}

int is_valid(ChessBoard board, Chess chess) {
    if (!is_in_board(chess.pos)) {
        printf("Out of board! Please drop on the board.\n");
        return 0;
    }
    else if (!is_empty(board, chess.pos)) {
        printf("The position is not empty! Please drop on an empty position.\n");
        return 0;
    }
    else if (chess.turn == 0 && !(chess.pos.x == SIZE/2 && chess.pos.y == SIZE/2)) {
        printf("The first move must be on the center of the board!\n");
        return 0;
    }
    else {
        return 1;
    }
}

int is_valid_silent(ChessBoard board, Chess chess) {
    return !(!is_in_board(chess.pos) || !is_empty(board, chess.pos) || (chess.turn == 0 && !(chess.pos.x == SIZE/2 && chess.pos.y == SIZE/2)));
}

Position rev_direc (Position direction) {
    Position rev_direction = {-direction.x, -direction.y};

    return rev_direction;
}

Position pos_make (int x, int y) {
    Position pos = {x, y};
    return pos;
}

Position pos_move (Position pos_at, Position direction) {

    pos_at.x += direction.x;
    pos_at.y += direction.y;

    return pos_at;
}

int is_end (ChessBoard board, Position pos, Position direction, int color) {

    // Close end
    if ((is_color(board, pos, color) && is_color(board, pos_move(pos, direction), -color)) // X(X)O
    || !is_in_board(pos_move(pos, direction))) { // X(X)|
        return 1;
    }

    // Open end
    else if (is_empty(board, pos) && !is_color(board, pos_move(pos, direction), color)) { // X(_)?
        return 1;
    }
    
    return 0;
}

Position move_to_end (ChessBoard board, Position pos_at, Position direction, int color) {
    while (!is_end(board, pos_at, direction, color)) {
        pos_at = pos_move(pos_at, direction);
    }

    return pos_at;
}