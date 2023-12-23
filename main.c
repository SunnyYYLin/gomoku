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

    ai1 = init_ai();
    ai2 = init_ai();

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

    // Restart
    char input;

    while (1) {
        printf("Press 'q' to quit or any other key to continue.\n");

        getchar();  // 读取换行符
        input = getchar();  // 读取一个字符

        // 忽略额外的字符，直到行结束
        while (input != '\n' && getchar() != '\n') { }

        if (input == 'q' || input == 'Q') {
            break;  // 如果输入是'q'或'Q'，退出循环
        }

        goto RESTART;
    }

    printf("Program exited.\n");
    return 0;
}
