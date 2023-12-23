#ifndef GAME_H
#define GAME_H

// Definition of the Player struct
typedef struct {
    int type;   // Type of player: Human, Random, or AI
    int color;  // Color of the player's pieces (Black or White)
    int score;  // Score of the player
} Player;

extern Player player1, player2;  // Declaration of two players

#include "shape.h"
#include "board.h"
#include "ai.h"

// Function declarations

// Prints information about a player
void print_player(Player player);

// Creates an empty player struct
Player empty_player();

// Initializes a player with a specified color
Player init_player(int color);

// Initializes the game environment
void init_game();

// Main game loop function
void play_game();

// Handles a player's move based on their type
void player_drop(Player player);

// Handles human player's move input
Position human_drop(Player player);

// Generates a random move for AI or random player
Position random_drop(Player player);

// Determines the game result based on the current board state
int game_referee(int** board, Position pos, int color);

// Displays the game result
void game_result(int turn, int referee);

#endif // GAME_H