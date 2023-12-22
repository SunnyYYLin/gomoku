#include "game.h"

#include "include.h"

Player empty_player() {
    Player player;
    player.type = 0;
    player.color = WHITE;
    player.score = 0;
    return player;
}

Player set_player() {
    Player player = empty_player();

    printf("Please choose your type [0 for human/1 for random/2 for ai]: \n");
    while (scanf("%d", player.type) != 1 || player.type < 0 || player.type > 2) {
        printf("Invalid input! Please try again.\n");
    }

    printf("Please choose your color [1 for black/-1 for white]: \n");
    while (scanf("%d", player.color) != 1 || player.color != BLACK || player.color != WHITE) {
        printf("Invalid input! Please try again.\n");
    }

    player.score = 0;
    printf("Player set.\n");
    print_player(player);
    return player;
}

Player print_player(Player player) {
    printf("Player type: %d\t", player.type);
    printf("Player color: %d\t", player.color);
    printf("Player score: %d\t", player.score);
    return player;
}

int is_player_set_valid(Player player1, Player player2) {
    if (player1.color != player2.color) {
        return 1;
    }
    else {
        printf("Player color cannot be the same!\n");
        return 0;
    }
}

void init_game() {
    board = empty_board();
    newChess = empty_chess();
    referee = 0;
    system("chcp 65001");
}