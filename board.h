# ifndef BOARD_H
# define BOARD_H

#include "symbol.h"
#include "position.h"

void init_board();
void print_board();
Position drop_board(int color);
void update_board(ChessColor color);

# endif // BOARD_H