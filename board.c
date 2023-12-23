#include "board.h"

#include <stdio.h>
#include <stdlib.h>

#include "global.h"

int **board;

void init_board(int*** board_ptr) {
    int **board_temp = (int **)malloc(SIZE * sizeof(int *));
    if (board_temp == NULL) {
        // 无法分配内存，处理错误，例如打印错误消息并退出程序
        fprintf(stderr, "Memory allocation failed for board_temp.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < SIZE; i++) {
        board_temp[i] = (int *)malloc(SIZE * sizeof(int));
        if (board_temp[i] == NULL) {
            // 无法分配内存，处理错误，释放之前已分配的内存并退出
            fprintf(stderr, "Memory allocation failed for board_temp[%d].\n", i);
            for (int j = 0; j < i; j++) {
                free(board_temp[j]); // 释放之前分配的行
            }
            free(board_temp); // 释放行指针数组
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < SIZE; j++) {
            board_temp[i][j] = EMPTY; // 假设 EMPTY 已经定义
        }
    }

    *board_ptr = board_temp; // 将分配好的棋盘地址赋值给外部变量
}

// Display the current state of the board
void print_board(int** board, Position pos_new, int turn) {
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
                    if (i == pos_new.x && j == pos_new.y) {
                        printf("▲");
                    }
                    else {
                        printf("●");
                    }
                    break;
                case WHITE:
                    if (i == pos_new.x && j == pos_new.y) {
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

int drop_board(int** board, Position pos, int color) {
    if (is_valid(board, pos, 0) == 1) {
        board[pos.x][pos.y] = color;
        return 1;
    }
    else {
        printf("drop_board: You can not drop at (%d, %d)!\n", pos.x, pos.y);
        return 0;
    }
    
}

void undo_board(int** board, Position pos) {
    board[pos.x][pos.y] = EMPTY;
}

// Boolean methods
int is_empty (int** board, Position pos) {
    int x = pos.x;
    int y = pos.y;

    if (is_in_board(pos)&&board[x][y] == EMPTY) {
        return 1;
    }

    return 0;
}

int is_color (int** board, Position pos, int color) {
    int x = pos.x;
    int y = pos.y;

    if (is_in_board(pos) && board[x][y] == color) {
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

int is_valid(int** board, Position pos, int is_silent) {
    if (!is_in_board(pos)) {
        if (!is_silent) printf("Out of board! Please drop on the board.\n");
        return -1;
    }
    else if (!is_empty(board, pos)) {
        if (!is_silent) printf("The position is not empty! Please drop on an empty position.\n");
        return -2;
    }
    else if (turn == 0 && !(pos.x == SIZE/2 && pos.y == SIZE/2)) {
        if (!is_silent) printf("The first move must be on the center of the board!\n");
        return -3;
    }
    else {
        return 1;
    }
}

int is_end (int** board, Position pos, Position direction, int color) {

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

int is_pos_equal(Position pos1, Position pos2) {
    if (pos1.x == pos2.x && pos1.y == pos2.y) {
        return 1;
    }

    return 0;
}

// Position methods

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

Position move_to_end (int** board, Position pos_at, Position direction, int color) {
    while (!is_end(board, pos_at, direction, color)) {
        pos_at = pos_move(pos_at, direction);
    }

    return pos_at;
}

Position* valid_positions(int** board, int color, int* count) {
    int initial_size = 10; // 初始大小
    int capacity = initial_size;
    Position* valid_pos = (Position*)malloc(sizeof(Position) * capacity);
    if (!valid_pos) {
        *count = 0;
        return NULL;
    }

    int num_valid_pos = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Position pos = {i, j};
            if (is_valid(board, pos, 1) == 1) {
                if (num_valid_pos == capacity) {
                    capacity *= 2; // 增加容量
                    Position* temp = (Position*)realloc(valid_pos, sizeof(Position) * capacity);
                    if (!temp) {
                        free(valid_pos);
                        *count = 0;
                        return NULL;
                    }
                    valid_pos = temp;
                }
                valid_pos[num_valid_pos++] = pos;
            }
        }
    }

    // 重新调整数组大小以匹配实际数量
    Position* resized_array = (Position*)realloc(valid_pos, sizeof(Position) * num_valid_pos);
    if (resized_array != NULL) {
        valid_pos = resized_array;
    }

    *count = num_valid_pos;
    return valid_pos;
}