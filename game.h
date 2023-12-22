#ifndef GAME_H
#define GAME_H

#include "shape.h"

typedef struct {
    int type;
    int color;
    int score;
} Player;

void print_player(Player player);
Player empty_player();
Player init_player(int color);
void init_game();
void play_game();
void player_drop(Player player);
Position human_drop(Player player);
Position random_drop(Player player);
// Position ai_drop(Player player);
int is_forbidden(int** board, Position pos, Player player);
void game_result(int turn, int referee);

#endif // GAME_H