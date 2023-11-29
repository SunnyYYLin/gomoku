#include "position.h"

#include "symbol.h"

extern ChessColor board[SIZE][SIZE];

Directions directions = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

int is_empty (Position pos) {
    int x = pos.x;
    int y = pos.y;

    if (board[x][y] == EMPTY) {
        return 1;
    }

    return 0;
}

int is_color (Position pos, ChessColor color) {
    int x = pos.x;
    int y = pos.y;

    if (board[x][y] == color) {
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

int is_valid (Position pos) {
    if (is_in_board(pos) && is_empty(pos)) {
        return 1;
    }

    return 0;
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

int is_end (ChessColor color, Position pos, Position direction) {

    // Close end
    if ((is_color(pos, color) && is_color(pos_move(pos, direction), -color)) // X(X)O
    || !is_in_board(pos_move(pos, direction))) { // X(X)|
        return 1;
    }

    // Open end
    else if (is_empty(pos) && !is_color(pos_move(pos, direction), color)) {
        return 1;
    }
    
    return 0;
}

Position move_to_end (ChessColor color, Position pos_at, Position direction) {
    while (is_in_board(pos_at) && !is_end(color, pos_at, direction)) {
        pos_at = pos_move(pos_at, direction);
    }

    return pos_at;
}