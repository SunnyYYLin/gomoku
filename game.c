#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

extern Player player1, player2;
extern int **board;
const char* colorText[3] = {"White", "Empty", "Black"};
const char* playerType[3] = {"Human", "Random", "AI"};
Position pos_new;
int turn;
int referee;

Player empty_player() {
    Player player;
    player.type = 0;
    player.color = WHITE;
    player.score = 0;
    return player;
}

Player init_player(int color) {
    Player player = empty_player();

    printf("Please choose your type [0 for human/1 for random/2 for ai] for %s: \n", colorText[color+1]);
    while (scanf("%d", &player.type) != 1 || player.type < 0 || player.type > 2) {
        printf("Invalid input! Please try again.\n");
        scanf("%*[^\n]"); // 清除错误输入
    }

    player.color = color;
    player.score = 0;
    printf("Player set.\n");
    print_player(player);
    return player;
}

void print_player(Player player) {
    printf("Player type: %s\t", playerType[player.type]);
    printf("Player color: %s\t", colorText[player.color+1]);
    printf("Player score: %d\n", player.score);
}

void init_game() {
    init_board(&board);
    turn = 0;
    referee = 0;
    system("chcp 65001");
}

void play_game() {
    if (turn == 0) {
        system("cls");
        print_board(board, pos_new, turn);
    }

    if (turn % 2 == 0) {
        player_drop(player1);
        referee = is_forbidden(board, pos_new, player1);
    } else {
        player_drop(player2);
        referee = is_forbidden(board, pos_new, player2);
    }
    system("cls");
    print_board(board, pos_new, turn);
}

void player_drop(Player player) {
    Position pos;
    switch (player.type) {
        case 0:
            pos_new = human_drop(player);
            break;
        case 1:
            pos_new = random_drop(player);
            break;
        // case 2:
        //     pos = ai_drop(player);
        //     break;
        default:
            printf("Invalid player type!\n");
            break;
    }
    drop_board(board, pos_new, player.color);
}

Position human_drop(Player player) {
    int x, y;
    char colChar;

    printf("Round %d is %s's turn. Drop on the board (example: B4). Type in 'X1' to undo:\n", turn, colorText[player.color + 1]);
    
    do {
        if (scanf(" %c%d", &colChar, &x) != 2) {
            // 输入格式错误，清除缓冲区并提示重新输入
            scanf("%*[^\n]"); // 清除错误输入
            scanf("%*c"); // 清除换行符
            printf("Invalid input format! Please use the format like 'B4'.\n");
            continue;
        }

        if (colChar == 'X') {
            return (Position){-1, -1};
        }

        pos_new = (Position){x - 1, toupper(colChar) - 'A'};
    } while (is_valid(board, pos_new) != 1);

    return pos_new;
}


Position random_drop(Player player) {
    int count = 0;
    Position valid_positions[SIZE*SIZE];

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == EMPTY) {
                valid_positions[count] = (Position){i, j};
                count++;
            }
        }
    }

    if (count == 0) {
        return (Position){-1, -1};
    }

    int random = rand() % count;
    return valid_positions[random];
}

// Position ai_drop(Player player) {
//     // AI 下棋逻辑
// }

int is_forbidden(int** board, Position pos, Player player) {
    enroll_lines(board, pos, player.color);
    Shape sum_shape = sum_lines();
    if (sum_shape.fives > 0) {
        return -1;
    }
    else if (player.color == BLACK && (sum_shape.open_threes+sum_shape.broken_threes > 1 || sum_shape.broken_fours+sum_shape.open_fours > 1 || sum_shape.longs >0)) {
        return 1;
    }
    else {
        return 0;
    }
}

void game_result(int turn, int referee) {
    if (referee == 0) {
        printf("It's a draw! Game over.\n");
    }
    else if (referee == -1) {
        printf("%s wins! Game over.\n", colorText[-2*(turn%2-1)]);
    }
    else if (referee == 1) {
        printf("Black had a forbbiden move! Game over.\n");
    }
}