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
double linearNeuron_forward(LinearNeuron neuron, Tensor input);

typedef struct BatchNorm {
    int input_size;
    double* gamma;
    double* beta;
    double* epsilon;
} BatchNorm;

BatchNorm batchNorm_init(double epsilon);
void batchNorm_free(BatchNorm batchnorm);
void batchNorm_print(BatchNorm batchnorm);
Tensor batchNorm_forward(BatchNorm batchNorm, Tensor input);

Tensor nonlinear(int nonlinearity_type, Tensor input);
Tensor ReLU(Tensor input);

typedef struct ConvLayer {
    int layer_size;
    int kernel_size;
    int stride;
    ConvNeuron* convNeurons;
} ConvLayer;

typedef struct LinearLayer {
    int input_size;
    int layer_size;
    LinearNeuron* linearNeurons;
} LinearLayer;

typedef struct ConvBlock {
    ConvLayer convLayer;
    BatchNorm batchNorm;
    int nonlinearity_type;
} ConvBlock;

typedef struct LinearBlock {
    LinearLayer linearLayer;
    BatchNorm batchNorm;
    int nonlinearity_type;
} LinearBlock;

typedef struct ResBlock {
    int num_convLayers;
    ConvBlock* convBlock;
    ConvLayer convLayer;
    BatchNorm batchNorm;
    int nonlinearity_type;
} ResBlock;

#endif // NN_H