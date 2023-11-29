# ifndef BOARD_H
# define BOARD_H

#define EMPTY 0
#define BLACK 1
#define WHITE -1

#define SIZE 15 // The size of the board



typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    int turn;
    int color;
    Position pos;
    int is_new;
} Chess;

typedef struct {
    Chess board[SIZE][SIZE];
} ChessBoard;

typedef Position Directions[4];
extern Directions directions;


// chess.c
Chess empty_chess();
int chess_color(Chess chess);
Chess make_chess(int color, Position pos);
Position chess_human_drop(Chess newChess);
Position chess_random_drop(ChessBoard board, Chess newChess);
extern const char* colorText[3];

// board.c
ChessBoard empty_board();
void print_board(ChessBoard board);
ChessBoard update_board(ChessBoard board, Chess chess);

// position.c
int is_empty(ChessBoard board, Position pos);
int is_color(ChessBoard board, Position pos, int color);
int is_center(Position pos);
int is_in_board(Position pos);
int is_on_edge(Position pos);
int is_on_corner(Position pos);
int is_available(ChessBoard board, Position pos);
int is_valid(ChessBoard board, Chess chess);
int is_valid_silent(ChessBoard board, Chess chess);
Position rev_direc(Position direction);
Position pos_make(int x, int y);
Position pos_move(Position pos, Position direction);
int is_end(ChessBoard board, Position pos, Position direction, int color);
Position move_to_end(ChessBoard board, Position pos, Position direction, int color);

#endif // POSITION_H