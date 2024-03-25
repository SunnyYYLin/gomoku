#include "tensor.h"
#include <stdlib.h>
#include <string.h>

Tensor tensor_init(int depth, int height, int width) {
    Tensor tensor;
    tensor.depth = depth;
    tensor.height = height;
    tensor.width = width;
    tensor.data = (double***)malloc(depth * sizeof(double**));
    for (int i = 0; i < depth; i++) {
        tensor.data[i] = (double**)malloc(height * sizeof(double*));
        for (int j = 0; j < height; j++) {
            tensor.data[i][j] = (double*)malloc(width * sizeof(double));
        }
    }
    return tensor;
}

void tensor_free(Tensor tensor) {
    for (int i = 0; i < tensor.depth; i++) {
        for (int j = 0; j < tensor.height; j++) {
            free(tensor.data[i][j]);
        }
        free(tensor.data[i]);
    }
    free(tensor.data);
}

void tensor_print(Tensor tensor) {
    for (int i = 0; i < tensor.depth; i++) {
        printf("depth %d:\n", i);
        for (int j = 0; j < tensor.height; j++) {
            for (int k = 0; k < tensor.width; k++) {
                printf("%f ", tensor.data[i][j][k]);
            }
            printf("\n");
        }
    }
}

void tensor_set_all(Tensor tensor, double value) {
    for (int i = 0; i < tensor.depth; i++) {
        for (int j = 0; j < tensor.height; j++) {
            for (int k = 0; k < tensor.width; k++) {
                tensor.data[i][j][k] = value;
            }
        }
    }
}

void tensor_random(Tensor tensor) {
    for (int i = 0; i < tensor.depth; i++) {
        for (int j = 0; j < tensor.height; j++) {
            for (int k = 0; k < tensor.width; k++) {
                tensor.data[i][j][k] = (double)rand() / RAND_MAX;
            }
        }
    }
}

Tensor tensor_pad(Tensor input, int padding) {
    int padded_height = input.height + 2 * padding;
    int padded_width = input.width + 2 * padding;

    Tensor padded_input = tensor_init(input.depth, padded_height, padded_width);
    tensor_set_all(padded_input, 0.0);

    for (int d = 0; d < input.depth; d++) {
        for (int i = 0; i < input.height; i++) {
            for (int j = 0; j < input.width; j++) {
                padded_input.data[d][i + padding][j + padding] = input.data[d][i][j];
            }
        }
    }
    tensor_free(input);

    return padded_input;
}

Tensor tensor_slice(Tensor input, int start, int end) {
    if (end-start > input.depth) {
        printf("Error: slice size is larger than input depth\n");
        exit(1);
    }
    Tensor output = tensor_init(end-start, input.height, input.width);
    for (int d = 0; d < end-start; d++) {
        memcpy(output.data[d], input.data[d+start], input.height * input.width * sizeof(double));
    }
    tensor_free(input);
    return output;
}

Tensor tensor_concatenate(Tensor input1, Tensor input2) {
    Tensor output = tensor_init(input1.depth + input2.depth, input1.height, input1.width);
    for (int d = 0; d < input1.depth; d++) {
        memcpy(output.data[d], input1.data[d], input1.height * input1.width * sizeof(double));
    }
    for (int d = 0; d < input2.depth; d++) {
        memcpy(output.data[d+input1.depth], input2.data[d], input2.height * input2.width * sizeof(double));
    }
    tensor_free(input1);
    tensor_free(input2);
    return output;
}

void tensor_embed(Tensor bed, Tensor input, int start) {
    for (int d = start; d < start + input.depth; d++) {
        memcpy(bed.data[d], input.data[d], input.height * input.width * sizeof(double));
    }
    tensor_free(input);
}