#ifndef TENSOR_H
#define TENSOR_H

typedef struct Tensor {
    union
    {
        struct {
            int depth;
            int height;
            int width;
        };
        int shape[3];
    };
    double*** data;
} Tensor;

Tensor tensor_init(int depth, int height, int width);
void tensor_free(Tensor tensor);
void tensor_print(Tensor tensor);
void tensor_set_all(Tensor tensor, double value);
void tensor_random(Tensor tensor);
Tensor tensor_pad(Tensor input, int padding);
Tensor tensor_slice(Tensor input, int start, int end);
Tensor tensor_concatenate(Tensor input1, Tensor input2);
void tensor_embed(Tensor bed, Tensor input, int start);

#endif // TENSOR_H