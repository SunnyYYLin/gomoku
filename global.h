#ifndef GLOBAL_H
#define GLOBAL_H

#include "include.h"
#include "game.h"
#include "board.h"

extern const char* colorText[3];
extern int** board;
extern int turn;
extern int referee;
extern Position pos_new;
extern Player player1, player2;
extern Position directions[4];
extern AI ai1, ai2;

#endif // GLOBAL_H