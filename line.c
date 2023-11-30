#include "referee.h"

#include "board.h"
#include <stdio.h>

Shape empty_shape() {
    Shape shape;
    shape.fives = 0;
    shape.longs = 0;
    shape.open_fours = 0;
    shape.broken_fours = 0;
    shape.open_threes = 0;
    shape.broken_threes = 0;
    return shape;
}

Line empty_line() {
    Line line;
    line.is_start_open = 0;
    line.is_end_open = 0;
    for (int i = 0; i < SIZE; i++) {
        line.segs[i] = 0;
    }
    line.shape = empty_shape();
    return line;
}

int fill_segs (ChessBoard board, Position pos_at, int color) {
    int value;

    if (is_color(board, pos_at, color)) {
        value = 1;
    }
    else if (is_empty(board, pos_at)) {
        value = 0;
    }
    else {
        value = -1;
    }

    return value;
}

Line get_line (ChessBoard board, int color, Position pos_at, Position direction) {
    Line line = empty_line();
    int i = 0;
    int is_in_seg = 0;
    int seg_length = 0;

    // printf("Getting line in direction (%d,%d) starting from (%d,%c)\n", direction.x, direction.y, pos_at.x+1, pos_at.y+'A');

    do {
        // printf("At position (%d,%c), ", pos_at.x+1, pos_at.y+'A');
        switch (i) {
            case 1:
                line.is_start_open = is_empty(board, pos_at);
            default:
                line.segs[i] = fill_segs(board, pos_at, color);
                i++;
                pos_at = pos_move(pos_at, direction);
        }
    }
    while (!is_end(board, pos_at, direction, color));

    line.is_end_open = is_empty(board, pos_at);
    line.segs[i] = fill_segs(board, pos_at, color);
    line.segs[i+1] = fill_segs(board, pos_move(pos_at, direction), color);
    line.segs[i+2] = -2;

    // printf("Line: ");
    // for (int j = 0; j <= i+1; j++) {
    //     printf("%d ", line.segs[j]);
    // }
    // printf(" start_open=%d, end_open=%d\n", line.is_start_open, line.is_end_open);

    return line;
}

Line num_fives_and_longs(Line line) {
    int i = 0;
    int count = 0;
    int is_in_seg = 0;

    do {
        if (is_in_seg && line.segs[i] == 1) {
            count ++;
        }
        else if (is_in_seg && line.segs[i] != 1) {
            if (count == 5) {
                line.shape.fives ++;
            }
            else if (count > 5) {
                line.shape.longs ++;
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
    while (line.segs[i] != -2);
    
    return line;
}

Line num_open_fours(Line line) {
    int is_seg_start_open = 0, is_seg_end_open = 0, is_in_seg = 0;
    int count = 0;
    int i = 0;

    do {
        if (is_in_seg && line.segs[i] == 1) {
            count ++;
        }
        else if (is_in_seg && line.segs[i] != 1) {
            if (line.segs[i] == 0 && line.segs[i+1] != 1) {
                is_seg_end_open = 1;
            }
            if (count == 4 && is_seg_start_open && is_seg_end_open) {
                line.shape.open_fours ++;
            }
            is_seg_start_open = 0;
            is_seg_end_open = 0;
            is_in_seg = 0;
            count = 0;
        }
        else if (!is_in_seg && line.segs[i] == 1) {
            if (i-1 >= 0 && line.segs[i-1] == 0 && (i-2<0 || line.segs[i-2] != 1)) {
                is_seg_start_open = 1;
            }
            is_in_seg = 1;
            count ++;
        }
        i ++;
    }
    while (line.segs[i] != -2);

    return line;
}

Line num_broken_fours(Line line) {
    int i = 0;
    int num_to_be_five = 0;

    do {
        if (line.segs[i] == 0) {
            Line test_line = line;
            test_line.segs[i] = 1;
            num_to_be_five += num_fives_and_longs(test_line).shape.fives - line.shape.fives;
        }
        i ++;
    }
    while (line.segs[i] != -2);

    line.shape.broken_fours = num_to_be_five - 2*line.shape.open_fours;

    return line;
}

Line num_open_threes(Line line) {
    int is_seg_start_open = 0, is_seg_end_open = 0, is_in_seg = 0;
    int count = 0;
    int i = 0;

    do {
        if (is_in_seg && line.segs[i] == 1) {
            count ++;
        }
        else if (is_in_seg && line.segs[i] != 1) {
            if (line.segs[i] == 0 && line.segs[i+1] != 1) {
                is_seg_end_open = 1;
            }
            if (count == 3 && is_seg_start_open && is_seg_end_open) {
                line.shape.open_threes ++;
            }
            is_seg_start_open = 0;
            is_seg_end_open = 0;
            is_in_seg = 0;
            count = 0;
        }
        else if (!is_in_seg && line.segs[i] == 1) {
            if (i-1 >= 0 && line.segs[i-1] == 0 && (i-2<0 || line.segs[i-2] != 1)) {
                is_seg_start_open = 1;
            }
            is_in_seg = 1;
            count ++;
        }
        i ++;
    }
    while (line.segs[i] != -2);

    return line;
}

Line num_broken_threes(Line line) {
    int i = 0;
    int num_to_be_open_fours = 0;

    do {
        if (line.segs[i] == 0) {
            Line test_line = line;
            test_line.segs[i] = 1;
            num_to_be_open_fours += (num_open_fours(test_line).shape.open_fours - line.shape.open_fours > 0)?(num_open_fours(test_line).shape.open_fours - line.shape.open_fours):0;
        }
        i ++;
    }
    while (line.segs[i] != -2);

    line.shape.broken_threes = num_to_be_open_fours - 2*line.shape.open_threes;

    return line;
}

Line line_shape(Line line) {
    line = num_fives_and_longs(line);
    line = num_open_fours(line);
    line = num_broken_fours(line);
    line = num_open_threes(line);
    line = num_broken_threes(line);
    
    return line;
}

Shape sum_lines(ChessBoard board, Chess chess) {
    Shape sum_shape = empty_shape();
    Position pos_at;

    // printf("Analyzing lines from position (%d,%c)\n", chess.pos.x+1, chess.pos.y+'A');
    for (int i = 0; i < 4; i++) {
        pos_at = chess.pos;
        pos_at = move_to_end(board, chess.pos, directions[i], chess.color);
        Line line = get_line(board, chess.color, pos_move(pos_at, directions[i]), rev_direc(directions[i]));
        line = line_shape(line);
        
        sum_shape.fives += line.shape.fives;
        sum_shape.longs += line.shape.longs;
        sum_shape.open_fours += line.shape.open_fours;
        sum_shape.broken_fours += line.shape.broken_fours;
        sum_shape.open_threes += line.shape.open_threes;
        sum_shape.broken_threes += line.shape.broken_threes;
    }

    return sum_shape;
}