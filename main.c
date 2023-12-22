#include "include.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Global variables
#include "global.h"
Player player1, player2;
int quit;

int main() {
    srand(time(NULL));

    // Choose player set
    player1 = init_player(BLACK);
    player2 = init_player(WHITE);

    // Initialize
    RESTART:
    init_game();

    // Play
    for (turn = 0; turn < SIZE*SIZE && referee == 0; turn++) {
        play_game();
        if (referee != 0) {
            break;
        }
    }
    game_result(turn, referee);

    // If quit?
    printf("Press any key to continue, or press 'q' to quit.\n");

    // 读取用户输入的字符
    char input;
    scanf("%c", &input);

    // 检查输入是否为 'q' 或 'Q'
    if (input == 'q' || input == 'Q') {
        return 0;
    } else {
        goto RESTART;
    }
}
