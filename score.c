// Calculates the score for a given shape.
int score_shape(Shape shape, int color, AI ai) {
    int score = 0;
    score += shape.fives * ai.score_five;
    score += shape.open_fours * ai.score_open_four;
    score += shape.broken_fours * ai.score_broken_four;
    score += shape.open_threes * ai.score_open_three;
    score += shape.broken_threes * ai.score_broken_three;
    score += shape.open_twos * ai.score_open_two;
    score += shape.broken_twos * ai.score_broken_two;
    return score;
}

// Evaluates scores for all valid moves.
int* evaluate(int** board, int color, Position* valid_pos, int valid_count, AI ai) {
    Position pos;
    int* scores = (int*)malloc(sizeof(int) * valid_count);

    for (int k=0; k<valid_count; k++) {
        pos = valid_pos[k];
        scores[k] = evaluate_move(board, pos, color, ai) + evaluate_move(board, pos, -color, ai);
    }

    return scores;
}

// Evaluates the score of a single move.
int evaluate_move(int** board, Position pos, int color, AI ai) {
    int** board_copy = copy_board(board);
    board_copy[pos.x][pos.y] = color;

    int score = 0;
    Shape* shapes = enroll_lines(board_copy, pos, color);
    Shape shape = sum_lines(shapes);
    if (is_win(shape)) {
        score = 10*ai.score_five;
    }
    else if (is_forbidden(shape, color)) {
        score = -10*ai.score_five;
    }
    else {
        score = score_shape(shape, color, ai);
    }

    free_board(board_copy);
    return score;
}

int evaluate_board(Position* valid_pos, int* scores, int valid_count) {
    int self_score = 0;
    int opponent_score = 0;

    for (int i=0; i<valid_count; i++) {
        if (scores[i] > opponent_score) {
            opponent_score = scores[i];
        }
    }

    return self_score - opponent_score;
}

int cmpfunc (const void * a, const void * b) {
   return ( -*(int*)a + *(int*)b );
}

Position* mostval_positions(Position* valid_pos, int* scores, int valid_count, AI ai, int n, int* mostval_count) {
    if (n > valid_count) n = valid_count;
    *mostval_count = n;

    // 分配内存给最终结果
    Position* mostval_positions = malloc(sizeof(Position) * n);

    // 创建索引数组
    int* indices = malloc(sizeof(int) * valid_count);
    for (int i = 0; i < valid_count; ++i) {
        indices[i] = i;
    }

    // 基于分数对索引进行排序
    for (int i = 0; i < valid_count - 1; ++i) {
        for (int j = 0; j < valid_count - i - 1; ++j) {
            if (scores[indices[j]] < scores[indices[j + 1]]) {
                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }

    // 根据排序后的索引选择最高分的位置
    for (int i = 0; i < n; ++i) {
        mostval_positions[i] = valid_pos[indices[i]];
    }

    return mostval_positions;
}

// Prints scores for each valid position on the board.
void print_scores(Position* valid_pos, int* score, int valid_count) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Position pos = pos_make(i, j);  // Assume pos_make creates a position
            int found = 0;
            for (int k = 0; k < valid_count; k++) {
                if (is_pos_equal(pos, valid_pos[k])) {  // Assume pos_equal compares two positions for equality
                    printf("%d ", score[k]);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("X ");
            }
        }
        printf("\n");
    }
}