#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

extern Player player1, player2;
extern int **board;
extern Strategy stg1, stg2;
static const char* colorText[3] = {"White", "Empty", "Black"};
static const char* playerType[3] = {"Human", "Random", "AI"};
static Position pos_new;
int turn;
int referee;

// Creates an empty player struct.
Player empty_player() {
    Player player;
    player.type = 0;  // Player type: Human, Random, AI
    player.color = WHITE;  // Player color
    player.score = 0;  // Player score
    return player;
}

// Initializes a player with a specified color.
Player init_player(int color) {
    Player player = empty_player();

    printf("Please choose your type [0 for human/1 for random/2 for ai] for %s: \n", colorText[color+1]);
    while (scanf("%d", &player.type) != 1 || player.type < 0 || player.type > 2) {
        printf("Invalid input! Please try again.\n");
        scanf("%*[^\n]");  // Clear erroneous input
    }

    player.color = color;
    player.score = 0;
    printf("Player set.\n");
    print_player(player);
    return player;
}

// Prints player information.
void print_player(Player player) {
    printf("Player type: %s\t", playerType[player.type]);
    printf("Player color: %s\t", colorText[player.color+1]);
    printf("Player score: %d\n", player.score);
}

// Initializes the game environment.
void init_game() {
    init_board(&board);
    turn = 0;
    referee = 0;
    system("chcp 65001");  // Set character encoding to UTF-8
}

// Main game loop.
void play_game() {
    system("cls");
    print_board(board, pos_new, turn);
    Player player = (turn % 2 == 0) ? player1 : player2;
    player_drop(player);
    system("cls");
    print_board(board, pos_new, turn);
    referee = game_referee(board, pos_new, player.color);
}

// Handles a player's move.
void player_drop(Player player) {
    Position pos;
    do {
        switch (player.type) {
            case 0:
                pos_new = human_drop(player);
                break;
            case 1:
                pos_new = random_drop(player);
                break;
            case 2:
                pos_new = ai_drop(board, player.color, stg1);
                break;
            case 3:
                pos_new = ai_drop(board, player.color, stg2);
                break;
            default:
                printf("Invalid player type!\n");
                break;
        }
    }
    while (!drop_board(board, pos_new, player.color));
}

// Handles human player's move.
Position human_drop(Player player) {
    int x, y;
    char colChar;

    printf("Round %d is %s's turn. Drop on the board (example: B4). Type in 'X1' to undo:\n", turn, colorText[player.color + 1]);
    
    if (scanf(" %c%d", &colChar, &x) != 2) {
        // Clear input buffer on incorrect format and prompt again
        scanf("%*[^\n]");  // Clear erroneous input
        scanf("%*c");  // Clear newline character
        printf("Invalid input format! Please use the format like 'B4'.\n");
        return (Position){-1, -1};
    }

    if (colChar == 'X') {
        return (Position){'X', -1};
    }

    pos_new = (Position){x - 1, toupper(colChar) - 'A'};

    return pos_new;
}

// Random move generator for AI or random player.
Position random_drop(Player player) {
    int valid_count = 0;
    Position* valid_pos = valid_positions(board, player.color, &valid_count);

    if (valid_count == 0) {
        printf("random_drop: No valid position!\n");
        return (Position){-1, -1};
    }
    else {
        int random_index = rand() % valid_count;
        Position pos = valid_pos[random_index];
        free(valid_pos);
        return pos;
    }
}

// Determines the game result based on the current board state.
int game_referee(int** board, Position pos, int color) {
    undo_board(board, pos);
    Shape* shapes = enroll_lines(board, pos, color);
    Shape init_sum_shape = sum_lines(shapes);
    drop_board(board, pos, color);
    shapes = enroll_lines(board, pos, color);
    Shape sum_shape = sum_lines(shapes);
    sum_shape = steply_shape(sum_shape, init_sum_shape);
    print_shape(sum_shape);
    if (is_win(sum_shape)) {
        return -1;  // Win condition
    }
    else if (is_forbidden(sum_shape, color)) {
        return 1;   // Forbidden move condition
    }
    else {
        return 0;   // Continue playing
    }
}

// Displays the game result.
void game_result(int turn, int referee) {
    int color = -2*(turn%2-1)-1;
    print_board(board, pos_new, turn);
    if (referee == 0) {
        printf("It's a draw! Game over.\n");
    }
    else if (referee == -1) {
        printf("%s wins! Game over.\n", colorText[color+1]);
    }
    else if (referee == 1) {
        printf("Black had a forbidden move! Game over.\n");
    }
}
