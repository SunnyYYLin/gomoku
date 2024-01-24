#include "game.h"

extern Player player1, player2;
extern int **board;
extern Strategy stg1, stg2;
static const char* colorText[3] = {"White", "Empty", "Black"};
static const char* playerType[3] = {"Human", "Random", "AI"};
Position pos_new;
int turn;
int referee;
char filename[100];

// Creates an empty player struct.
Player empty_player() {
    Player player;
    player.type = 0;  // Player type: Human, Random, AI
    player.color = WHITE;  // Player color
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
    printf("Player set.\n");
    print_player(player);
    return player;
}

// Prints player information.
void print_player(Player player) {
    printf("Player type: %s\t", playerType[player.type]);
    printf("Player color: %s\n", colorText[player.color+1]);
}

// Initializes the game environment.
void init_game() {
    init_board(&board);
    turn = 0;
    referee = 0;
    generate_filename(filename, sizeof(filename));
    system("chcp 65001");  // Set character encoding to UTF-8
}

// Main game loop.
void play_game() {
    print_board(board, pos_new, turn);
    Player player = player_drop();
    system("cls");
    referee = game_referee(board, pos_new, player.color);
    record_step();
}

// Handles a player's move.
Player player_drop() {
    Position pos;
    Player player;
    do {
        UNDO:
        player = (turn % 2 == 0) ? player1 : player2;
        switch (player.type) {
            case 0:
                pos = human_drop(player);
                if (pos.y == 'X') {
                    undo_board(board, pos_new);
                    turn --;
                    goto UNDO;
                }
                pos_new = pos;
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
    return player;
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
        return (Position){1, 'X'};
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
    Shape* shapes = enroll_lines(board, pos, color);
    Shape sum_shape = sum_lines(shapes);
    // print_shape(sum_shape);
    // getchar();
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

void generate_filename(char *buffer, size_t buffer_size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    strftime(buffer, buffer_size, "%H%M%S.txt", tm_info);
}

void record_step() {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "%c,%d\n", pos_new.y+'A', pos_new.x+1);
    fclose(file);
    printf("Step recorded in %s\n", filename);
}

int load_game() {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    int x, y = 0;
    init_board(&board);
    referee = 0;
    turn = 0;

    while (fscanf(file, "%c,%d\n", &y, &x) == 2) {
        pos_new.x = x - 1;
        pos_new.y = y - 'A';
        drop_board(board, pos_new, (turn % 2 == 0)?BLACK:WHITE);
        turn ++;
    }

    fclose(file);
    system("chcp 65001");  // Set character encoding to UTF-8
    return 1;
}