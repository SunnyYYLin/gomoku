#ifndef NN_H
#define NN_H

#include "tensor.h"

#ifndef max(a,b)
    #define max(a,b) (((a)>(b))?(a):(b))
#endif

typedef struct ConvNeuron {
    int kernel_size;
    int stride;
    Tensor weights;
    double* bias;
} ConvNeuron;

ConvNeuron convNeuron_init(int kernel_size, int stride);
void convNeuron_free(ConvNeuron neuron);
void convNeuron_print(ConvNeuron neuron);
Tensor convNeuron_forward(ConvNeuron neuron, Tensor input);

typedef struct LinearNeuron {
    int input_size;
    Tensor weights;
    double* bias;
} LinearNeuron;

LinearNeuron linearNeuron_init(int input_size);
void linearNeuron_free(LinearNeuron neuron);
void linearNeuron_print(LinearNeuron neuron);
Tensor linearNeuron_forward(LinearNeuron neuron, Tensor input);

typedef struct BatchNorm {
    int input_size;
    Tensor gamma;
    Tensor beta;
    double* epsilon;
} BatchNorm;

BatchNorm batchNorm_init(int depth, int height, int width);
void batchNorm_free(BatchNorm batchnorm);
void batchNorm_print(BatchNorm batchnorm);
Tensor batchNorm_forward(BatchNorm batchNorm, Tensor input);

Tensor ReLU(Tensor input);

typedef struct ConvLayer {
    int input_size;
    int output_size;
    ConvNeuron* convneurons;
} ConvLayer;

typedef struct LinearLayer {
    int input_size;
    int output_size;
    LinearNeuron* linearneurons;
} LinearLayer;

typedef struct ConvBlock {
    int input_size;
    int output_size;
    ConvLayer* convlayer;
    BatchNorm* batchnorm;
} ConvBlock;

#endif // NN_H