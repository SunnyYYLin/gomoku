#include "shape.h"
#include <stdio.h>
#include <stdlib.h>

// Array representing the four directions to check on the board
static const Position directions[4] = {{0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};

// Prints a shape
void shape_print(Shape shape) {
    for (int i = 0; i < SHAPE_SIZE; i++) {
        printf("%d, ", shape.arr[i]);
    }
    printf("\n");

    return;
}

// Creates an empty shape struct
Shape shape_init() {
    Shape shape;

    // Initializing all shape attributes to zero
    for (int i=0; i<SHAPE_SIZE; i++) {
        shape.arr[i] = 0;
    }

    return shape;
}

// Creates an empty line struct
Line line_init() {
    Line line;
    // Initializing line attributes, including setting its shape to empty
    line.len = 0;
    for (int i = 0; i < SIZE; i++) {
        line.segs[i] = 0;
    }
    line.shape = empty_shape();
    return line;
}

// Fills segments in a line based on the board's state
int get_line_filled(int** board, Position pos_at, int color) {
    int value;
    if (is_color(board, pos_at, color)) {
        value = 1;  // Same color as the player
    } else if (is_empty(board, pos_at)) {
        value = 0;  // Empty spot
    } else {
        value = -1; // Opponent's color or out of board
    }
    return value;
}

// Moves a Position to the end of a line of pieces
Position move_to_end(int** board, Position pos_at, Position direction, int color) {
    int empty_count = 0;

    // OO_ _ (_); OOO(X); OOO(|)
    while (empty_count<SEG_GAP && !is_color(board, pos_at, -color) && is_in_board(pos_at)) {
        if (is_empty(board, pos_at)) {
            empty_count ++;
        } else {
            empty_count = 0;
        }
        pos_at = pos_move(pos_at, direction);
    }

    return pos_at;
}

// Retrieves a line of pieces from the board
Line get_line(int** board, int color, Position pos_at, Position direction) {
    Line line = empty_line(); // Start with an empty line
    int i = 0;
    int empty_count = 0;

    pos_at = pos_move(pos_at, direction); // _(_)_OO; X(O)OO; |(O)OO

    // Iterate until the end of the line is reached
    // OO_ _(_); OOO(X); OOO(|)
    while (empty_count<=SEG_GAP && !is_color(board, pos_at, -color) && is_in_board(pos_at)) {
        if (is_empty(board, pos_at)) {
            empty_count ++;
        }
        else {
            empty_count = 0;
        }
        line.segs[i] = fill_segs(board, pos_at, color);
        pos_at = pos_move(pos_at, direction);
        i ++;
    }

    line.len = i; // Length of the line

    return line;
}

Line num_fives_and_longs(Line line) {
    int i = 0;
    int count = 0;
    int is_in_seg = 0;
    int fives = 0;
    int longs = 0;

    do {
        if (is_in_seg && line.segs[i] == 1) {
            count ++;
        }
        else if (is_in_seg && line.segs[i] != 1) {
            if (count == 5) {
                fives ++;
            }
            else if (count > 5) {
                longs ++;
            }
            count = 0;
            is_in_seg = 0;
        }
        else if (!is_in_seg && line.segs[i] == 1) {
            is_in_seg = 1;
            count ++;
        }
        i ++;
    }
    while (i<=line.len);

    line.shape.fives = fives;
    line.shape.longs = longs;
    
    return line;
}

Line num_open_fours(Line line) {
    int is_seg_start_open = 0, is_seg_end_open = 0, is_in_seg = 0;
    int count = 0;
    int i = 0;
    int open_fours = 0;

    do {
        if (is_in_seg && line.segs[i] == 1) {
            count ++;
        }
        else if (is_in_seg && line.segs[i] != 1) {
            if (i<line.len && line.segs[i] == 0) {
                is_seg_end_open = 1;
            }
            if (count == 4 && is_seg_start_open && is_seg_end_open) {
                open_fours ++;
            }
            is_seg_start_open = 0;
            is_seg_end_open = 0;
            is_in_seg = 0;
            count = 0;
        }
        else if (!is_in_seg && line.segs[i] == 1) {
            if (i-1 >= 0 && line.segs[i-1] == 0) {
                is_seg_start_open = 1;
            }
            is_in_seg = 1;
            count ++;
        }
        i ++;
    }
    while (i<=line.len);

    line.shape.open_fours = open_fours;

    return line;
}

Line num_broken_fours(Line line) {
    int i = 0;
    int num_to_be_five = 0;
    line = num_fives_and_longs(line);
    line = num_open_fours(line);

    do {
        if (line.segs[i] == 0) {
            Line test_line = line;
            test_line.segs[i] = 1;
            test_line = num_fives_and_longs(test_line);
            num_to_be_five += max(test_line.shape.fives - line.shape.fives, 0);
        }
        i ++;
    }
    while (i<=line.len);

    line.shape.broken_fours = num_to_be_five - 2*line.shape.open_fours;

    return line;
}

// __OOO__
Line num_open_threes(Line line) {
    int open_count = 0;
    int is_in_seg = 0;
    int count = 0;
    int i = 0;
    int open_threes = 0;

    do {
        if (is_in_seg && line.segs[i] == 1) {
            count ++;
        }
        else if (is_in_seg && line.segs[i] != 1) {
            if (i < line.len && line.segs[i] == 0) {
                open_count ++;
                if (i+1 < line.len && line.segs[i+1] == 0) {
                    open_count ++;
                }
            }
            if (count == 3 && open_count >= 3) {
                open_threes ++;
            }
            open_count = 0;
            is_in_seg = 0;
            count = 0;
        }
        else if (!is_in_seg && line.segs[i] == 1) {
            if (i-1 >= 0 && line.segs[i-1] == 0) {
                open_count ++;
                if (i-2>=0 && line.segs[i-2] == 0) {
                    open_count ++;
                }
            }
            is_in_seg = 1;
            count ++;
        }
        i ++;
    }
    while (i<=line.len);

    line.shape.open_threes = open_threes;

    return line;
}

Line num_jump_threes(Line line) {
    int num_to_be_open_fours = 0;
    int count = 0;
    int i = 0;

    do {
        if (line.segs[i] == 0) {
            Line test_line = line;
            test_line.segs[i] = 1;
            test_line = num_open_fours(test_line);
            num_to_be_open_fours += max(test_line.shape.open_fours - line.shape.open_fours, 0);
        }
        i ++;
    }
    while (i<=line.len);

    line.shape.jump_threes = max(num_to_be_open_fours - 2*line.shape.open_threes, 0);

    return line;
}

Line line_shape(Line line) {
    if (line.len >= 5) {
        line = num_fives_and_longs(line);
        line = num_open_fours(line);
        line = num_broken_fours(line);
        line = num_open_threes(line);
        line = num_jump_threes(line);
    }

    return line;
}

Shape* enroll_lines(int** board, Position pos, int color) {
    Position pos_at;

    Shape* shapes = (Shape*)malloc(4*sizeof(Shape));
    // printf("Analyzing lines from position (%d,%c)\n", chess.pos.x+1, chess.pos.y+'A');
    for (int i = 0; i < 4; i++) {
        pos_at = pos;
        pos_at = move_to_end(board, pos, directions[i], color);
        Line line = get_line(board, color, pos_at, rev_direc(directions[i]));
        line = line_shape(line);
        shapes[i] = line.shape;
    }

    return shapes;
}

Shape sum_lines(Shape* shapes) {
    Shape sum_shape = empty_shape();
    Shape shape;
    
    for (int i = 0; i < 4; i++) {
        shape = shapes[i];
        for (int j = 0; j < SHAPE_SIZE; j++) {
            sum_shape.arr[j] += shape.arr[j];
        }
    }

    free(shapes);
    return sum_shape;
}

int is_forbidden(Shape sum_shape, int color) {
    if (color == BLACK && ((sum_shape.open_threes+sum_shape.jump_threes)> 1 || (sum_shape.broken_fours+sum_shape.open_fours) > 1 || sum_shape.longs >0)) {
        return 1;
    }
    else {
        return 0;
    }
}

int is_win(Shape sum_shape) {
    if (sum_shape.fives > 0) {
        return 1;
    }
    else {
        return 0;
    }
}