#ifndef GAME_H
#define GAME_H

#include "include.h"

typedef struct {
    int type;// 0: human, 1: random, 2: ai
    int color;
    int score;
} Player;

extern ChessBoard board;
extern Chess newChess;
extern Player player1, player2;
extern int quit;
extern int referee;

// player.c
Player empty_player();
Player set_player();
Player print_player(Player player);
int is_player_set_valid(Player player1, Player player2);

#endif // GAME_H