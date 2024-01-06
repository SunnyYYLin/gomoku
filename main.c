#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"
#include "game.h"
#include "score.h"

// Global variables
extern Strategy stg1, stg2;
extern int referee;
extern char filename[100];
Player player1, player2;
int quit;

int main() {
    srand(time(NULL));  // Seed the random number generator
    int input;

    // Initialize players
    player1 = init_player(BLACK);  // Initialize player1 with BLACK color
    player2 = init_player(WHITE);  // Initialize player2 with WHITE color

    // Initialize AI players
    stg1 = init_stg();  // Initialize first AI player
    stg2 = init_stg();  // Initialize second AI player

    // Main game loop
    RESTART:
    printf("Press 'n' to start a new game. Press 'l' to load from a file.\n");
    getchar();  // Read the newline character
    input = getchar();  // Read a single character

    if (input == 'l') {
        printf("Please type in filename to read game from: ");
        scanf(" %99s", filename);
        printf("Loading game from %s...\n", filename);
        load_game();
    } 
    else if (input == 'n') {
        init_game();
    }
    else {
        printf("Invalid input! Please try again.\n");
        goto RESTART;
    }

    // Play the game until there are no more moves or a referee decision is made
    for (/*turn is initialized before*/ ; turn < SIZE * SIZE && referee == 0; turn++) {
        play_game();  // Play a single turn of the game
        if (referee != 0) {
            break;  // Exit the loop if the game is over
        }
    }

    // Display the result of the game
    game_result(turn, referee);

    // Restart or quit option
    while (1) {
        printf("Press 'q' to quit or any other key to continue.\n");

        getchar();  // Read the newline character
        input = getchar();  // Read a single character

        // Check if the user wants to quit
        if (input == 'q' || input == 'Q') {
            break;  // Exit the loop if 'q' or 'Q' is entered
        }

        // Restart the game
        system("cls");
        goto RESTART;
    }

    printf("Program exited.\n");  // Print a message when exiting the program
    return 0;
}