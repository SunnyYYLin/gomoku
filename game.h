#ifndef GAME_H
#define GAME_H

typedef struct {
    int type;
    int color;
    int score;
} Player;

extern Player player1, player2;

#include "shape.h"
#include "board.h"
#include "ai.h"

void print_player(Player player);
Player empty_player();
Player init_player(int color);
void init_game();
void play_game();
void player_drop(Player player);
Position human_drop(Player player);
Position random_drop(Player player);
int game_referee(int** board, Position pos, int color);
void game_result(int turn, int referee);

#endif // GAME_H