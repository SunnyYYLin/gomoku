#include "line.h"

#include "position.h"

Shape empty_shape() {
    Shape shape;
    shape.fives = 0;
    shape.fours = 0;
    shape.live_threes = 0;
    shape.longs = 0;
    return shape;
}

Line empty_line() {
    Line line;
    line.start_open = 0;
    line.end_open = 0;
    for (int i = 0; i < 4; i++) {
        line.segs[i] = 0;
    }
    line.shape = empty_shape();
    return line;
}

Line get_line (ChessColor color, Position pos_at, Position direction) {
    Line line = empty_line();
    int i = 0;
    int in_seg = 0;
    int seg_length = 0;

    // printf("Getting line in direction (%d,%d) starting from (%d,%d)\n", direction.x, direction.y, pos_at.x, pos_at.y);

    while (is_in_board(pos_at)) {
        // printf("At position (%d,%d), ", pos_at.x, pos_at.y);
        if (!in_seg && is_color(pos_at, color)) {
            in_seg = 1;
            seg_length++;
            // printf("Starting new segment\n");
        }
        else if (in_seg && is_color(pos_at, color)) {
            seg_length++;
            // printf("Continuing segment, length now %d\n", seg_length);
        }
        else if (in_seg && is_empty(pos_at)) {
            // printf("Segment ended with length %d\n", seg_length);
            line.segs[i] = seg_length;
            if (i == 0) {
                line.start_open = 1;
            }
            in_seg = 0;
            seg_length = 0;
            i++;
        }
        pos_at = pos_move(pos_at, direction);
    }

    if (in_seg) {
        line.segs[i] = seg_length;
        // printf("Segment ended at border with length %d\n", seg_length);
    }
    line.end_open = is_empty(pos_at);

    // printf("Line: ");
    for (int j = 0; j <= i; j++) {
        // printf("%d ", line.segs[j]);
    }
    // printf(" start_open=%d, end_open=%d\n", line.start_open, line.end_open);

    return line;
}

int is_segs_start(int i) {
    if (i == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

int is_segs_end(Line line, int i) {
    if (line.segs[i+1] == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

int is_in_segs(Line line, int i) {
    if (line.segs[i] != 0) {
        return 1;
    }
    else {
        return 0;
    }
}

int is_segs_inside(Line line, int i) {
    if (is_in_segs(line, i) && !is_segs_start(i) &&!is_segs_end(line, i)) {
        return 1;
    }
    else {
        return 0;
    }
}

int num_fives(Line line) {
    int fives = 0;

    for (int i = 0; is_in_segs(line, i); i++) {
        if (line.segs[i] == 5) {
            fives ++;
        }
    }

    return fives;
}

int num_fours(Line line) {
    int fours = 0;

    for (int i = 0; is_in_segs(line, i); i++) {
        if ((line.segs[i] == 3 && line.segs[i+1] == 1) // XXX_X
        || (line.segs[i] == 1 && line.segs[i+1] == 3) // X_XXX
        || (line.segs[i] == 2 && line.segs[i+1] == 2)) // XX_XX
        {
            fours ++;
        }
        else if (line.segs[i] == 4)
        {
            if ((is_segs_start(i) && line.start_open) // _|XXXX
            || (is_segs_end(line, i) && line.end_open) // XXXX|_
            || is_segs_inside(line, i)) // _XXXX_
            {
                fours ++;
            }
        }
    }

    return fours;
}

int num_live_threes(Line line) {
    int live_threes = 0;

    for (int i = 0; is_in_segs(line, i); i++) {
        if (line.segs[i] == 3) {
            if ((is_segs_start(i) && !is_segs_end(line, i) && line.start_open) // _|XXX_
            || (is_segs_end(line, i) && !is_segs_start(i) && line.end_open) // _XXX|_
            || (is_segs_end(line, i) && is_segs_start(i) && line.start_open && line.end_open)) {  // _|XXX|_
                live_threes ++;
            }
        }
        else if (line.segs[i] == 1 && line.segs[i+1] == 2) {
            if (is_segs_inside(line, i) && is_segs_inside(line, i+1) // _X_XX_
            || (is_segs_end(line, i+1) && !is_segs_start(i) && line.end_open) // _X_XX|_
            || (is_segs_start(i) && !is_segs_end(line, i+1) && line.start_open) // _|X_XX_
            || (is_segs_end(line, i+1) && is_segs_start(i) && line.start_open && line.end_open)) {// _|X_XX|_
                live_threes ++;
            }
        }
        else if (line.segs[i+1] == 1 && line.segs[i] == 2) {
            if (is_segs_inside(line, i+1) && is_segs_inside(line, i) // _XX_X_
            || (is_segs_end(line, i+1) && !is_segs_start(i) && line.end_open) // _XX_X|_
            || (is_segs_start(i) && !is_segs_end(line, i+1) && line.start_open) // _|XX_X_
            || (is_segs_end(line, i+1) && is_segs_start(i) && line.start_open && line.end_open)) {// _|XX_X|_
                live_threes ++;
            }
        }
    }

    return live_threes;
}

int num_longs(Line line) {
    int longs = 0;

    for (int i = 0; is_in_segs(line, i) && i<=SIZE; i++) {
        if (line.segs[i] > 5) {
            longs ++;
        }
    }

    return longs;
}

Line line_shape(Line line) {
    line.shape.live_threes = num_live_threes(line);
    line.shape.fours = num_fours(line);
    line.shape.fives = num_fives(line);
    line.shape.longs = num_longs(line);
    
    return line;
}

Shape sum_lines(ChessColor color, Position pos) {
    Shape sum_shape = empty_shape();
    Line line;
    Position pos_at;

    // printf("Analyzing lines from position (%d,%d)\n", pos.x, pos.y);
    for (int i = 0; i < 4; i++) {
        pos_at = pos;
        pos_at = move_to_end(color, pos_at, directions[i]);
        line = get_line(color, pos_at, rev_direc(directions[i]));
        
        sum_shape.live_threes += num_live_threes(line);
        sum_shape.fours += num_fours(line);
        sum_shape.fives += num_fives(line);
        sum_shape.longs += num_longs(line);

        // printf("Direction %d: live_threes=%d, fours=%d, fives=%d, longs=%d\n", i, num_live_threes(line), num_fours(line), num_fives(line), num_longs(line));
    }
    // printf("Total: live_threes=%d, fours=%d, fives=%d, longs=%d\n", sum_shape.live_threes, sum_shape.fours, sum_shape.fives, sum_shape.longs);

    return sum_shape;
}