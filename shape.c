#include "shape.h"
#include <stdio.h>
#include <stdlib.h>

// Array representing the four directions to check on the board
static const Position directions[4] = {{0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};

// Prints a shape
void print_shape(Shape shape) {
    printf("Shape: %d longs, %d fives, %d open fours, %d broken fours, \n%d open threes, %d jump threes, %d broken threes, \n%d open twos, %d jump twos, %d djump twos, %d broken twos\n", 
    shape.longs, shape.fives, shape.open_fours, shape.broken_fours, shape.open_threes, shape.jump_threes, shape.broken_threes, shape.open_twos, shape.jump_twos, shape.djump_twos, shape.broken_twos);
}

// Creates an empty shape struct
Shape empty_shape() {
    Shape shape;
    // Initializing all shape attributes to zero
    shape.fives = 0;
    shape.longs = 0;
    shape.open_fours = 0;
    shape.broken_fours = 0;
    shape.open_threes = 0;
    shape.jump_threes = 0;
    shape.broken_threes = 0;
    shape.open_twos = 0;
    shape.jump_twos = 0;
    shape.djump_twos = 0;
    shape.broken_twos = 0;
    return shape;
}

// Creates an empty line struct
Line empty_line() {
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
int fill_segs(int** board, Position pos_at, int color) {
    int value;
    if (is_color(board, pos_at, color)) {
        value = 1;  // Same color as the player
    } else if (is_empty(board, pos_at)) {
        value = 0;  // Empty spot
    } else {
        value = -1; // Opponent's color or invalid
    }
    return value;
}

// Retrieves a line of pieces from the board
Line get_line(int** board, int color, Position pos_at, Position direction) {
    Line line = empty_line(); // Start with an empty line
    int i = 0;

    // Iterate until the end of the line is reached
    do {
        line.segs[i] = fill_segs(board, pos_at, color);
        i++;
        pos_at = pos_move(pos_at, direction);
    } while (!is_cut(board, pos_at, direction, color));

    line.segs[i] = fill_segs(board, pos_at, color);

    line.len = i + 1; // Length of the line

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

    line.shape.broken_fours = max(num_to_be_five - 2*line.shape.open_fours, 0);

    return line;
}

// __OOO__
Line num_open_threes(Line line, int* to_be_open_fours) {
    int open_count = *to_be_open_fours;
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
                *to_be_open_fours = open_count;
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
    int open_threes_to_open_fours = 0;
    line = num_open_fours(line);
    line = num_open_threes(line, &open_threes_to_open_fours);

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

    line.shape.jump_threes = max(num_to_be_open_fours - (open_threes_to_open_fours-2)* line.shape.open_threes, 0);

    return line;
}

Line num_broken_threes(Line line) {
    int num_to_be_broken_fours_r = 0;
    int num_to_be_broken_fours_l = 0;
    int i = 0;
    int temp;
    line = num_broken_fours(line);
    line = num_open_threes(line, &temp);
    line = num_jump_threes(line);

    do {
        if (line.segs[i] == 0 && line.segs[i+1] == 1) {
            Line test_line = line;
            test_line.segs[i] = 1;
            test_line = num_broken_fours(test_line);
            num_to_be_broken_fours_r += max(test_line.shape.broken_fours - line.shape.broken_fours, 0);
        }
        i ++;
    }
    while (i<=line.len);

    do {
        if (line.segs[i] == 0 && line.segs[i-1] == 1) {
            Line test_line = line;
            test_line.segs[i] = 1;
            test_line = num_broken_fours(test_line);
            num_to_be_broken_fours_l += max(test_line.shape.broken_fours - line.shape.broken_fours, 0);
        }
        i --;
    }
    while (i>=0);

    int num_to_be_broken_fours = max(num_to_be_broken_fours_r, num_to_be_broken_fours_l);

    line.shape.broken_threes = max(num_to_be_broken_fours/2 - (line.shape.open_threes+line.shape.jump_threes), 0);

    return line;
}

Line num_open_twos(Line line) {
    int is_seg_start_open = 0, is_seg_end_open = 0, is_in_seg = 0;
    int count = 0;
    int i = 0;
    int open_twos = 0;

    do {
        if (is_in_seg && line.segs[i] == 1) {
            count ++;
        }
        else if (is_in_seg && line.segs[i] != 1) {
            if ((i<line.len && line.segs[i] == 0) && (i+1<line.len && line.segs[i+1] == 0) && (i+2<line.len && line.segs[i+2] == 0)) {
                is_seg_end_open = 1;
            }
            if (count == 2 && is_seg_start_open && is_seg_end_open) {
                open_twos ++;
            }
            is_seg_start_open = 0;
            is_seg_end_open = 0;
            is_in_seg = 0;
            count = 0;
        }
        else if (!is_in_seg && line.segs[i] == 1) {
            if (i-1 >= 0 && line.segs[i-1] == 0 && (i-2 >= 0 && line.segs[i-2] == 0) && (i-3>=0 && line.segs[i-3] == 0)) {
                is_seg_start_open = 1;
            }
            is_in_seg = 1;
            count ++;
        }
        i ++;
    }
    while (i<=line.len);

    line.shape.open_twos = open_twos;

    return line;
}

Line num_jump_twos(Line line) {
    int num_to_be_open_threes = 0;
    int i = 0;
    int temp;
    line = num_open_threes(line, &temp);
    line = num_open_twos(line);

    do {
        if (line.segs[i] == 0 && line.segs[i+1] == 1) {
            Line test_line = line;
            test_line.segs[i] = 1;
            test_line = num_open_threes(test_line, &temp);
            num_to_be_open_threes += max(test_line.shape.open_threes - line.shape.open_threes, 0);
        }
        i++;
    }
    while (i<=line.len);

    line.shape.jump_twos = max(num_to_be_open_threes - 2*line.shape.open_twos, 0);

    return line;
}

Line num_djump_twos(Line line) {
    int num_to_be_jump_threes = 0;
    int i = 0;
    int temp;
    line = num_jump_threes(line);
    line = num_open_twos(line);
    line = num_jump_twos(line);

    do {
        if (line.segs[i] == 0 && line.segs[i+1] == 1) {
            Line test_line = line;
            test_line.segs[i] = 1;
            test_line = num_open_threes(test_line, &temp);
            num_to_be_jump_threes += max(test_line.shape.open_threes - line.shape.open_threes, 0);
        }
        i++;
    }
    while (i<=line.len);

    line.shape.djump_twos = max(num_to_be_jump_threes - 2*line.shape.jump_twos, 0);

    return line;
}

Line num_broken_twos(Line line) {
    int i = 0;
    int num_to_be_broken_threes = 0;
    line = num_broken_threes(line);
    line = num_open_twos(line);
    line = num_jump_twos(line);
    line = num_djump_twos(line);

    do {
        if (line.segs[i] == 0) {
            Line test_line = line;
            test_line.segs[i] = 1;
            test_line = num_broken_threes(test_line);
            num_to_be_broken_threes += max(test_line.shape.broken_threes - line.shape.broken_threes, 0);
        }
        i ++;
    }
    while (i<=line.len);

    line.shape.broken_twos = max(min(num_to_be_broken_threes,1), max(num_to_be_broken_threes/6 - 2*(line.shape.open_twos+line.shape.jump_twos+line.shape.djump_twos), 0));

    return line;
}

Line line_shape(Line line) {
    int temp;
    line = num_fives_and_longs(line);
    line = num_open_fours(line);
    line = num_broken_fours(line);
    line = num_open_threes(line, &temp);
    line = num_broken_threes(line);
    line = num_open_twos(line);
    line = num_broken_twos(line);
    
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
        sum_shape.fives += shape.fives;
        sum_shape.longs += shape.longs;
        sum_shape.open_fours += shape.open_fours;
        sum_shape.broken_fours += shape.broken_fours;
        sum_shape.open_threes += shape.open_threes;
        sum_shape.jump_threes += shape.jump_threes;
        sum_shape.broken_threes += shape.broken_threes;
        sum_shape.open_twos += shape.open_twos;
        sum_shape.broken_twos += shape.broken_twos;
    }

    free(shapes);
    return sum_shape;
}

Shape steply_shape(Shape shape, Shape init_shape) {
    for (int i = 0; i < STG_SIZE; i++) {
        shape.arr[i] -= init_shape.arr[i];
    }
    return shape;
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