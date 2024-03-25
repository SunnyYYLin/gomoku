#include "NN.h"
#include <stdlib.h>
#include <math.h>

/*-------------------------------------------------------------Convolutional Neuron------------------------------------------------------------------------*/

ConvNeuron convNeuron_init(int kernel_size, int stride) {
    ConvNeuron neuron;
    neuron.kernel_size = kernel_size;
    neuron.stride = stride;
    neuron.weights = tensor_init(1, kernel_size, kernel_size);
    tensor_random(neuron.weights);
    neuron.bias = (double*)malloc(sizeof(double));
    *neuron.bias = rand() / (double)RAND_MAX;
    return neuron;
}

void convNeuron_free(ConvNeuron neuron) {
    tensor_free(neuron.weights);
    free(neuron.bias);
}

void convNeuron_print(ConvNeuron neuron) {
    printf("kernel_size: %d\n", neuron.kernel_size);
    printf("stride: %d\n", neuron.stride);
    printf("weights:\n");
    tensor_print(neuron.weights);
    printf("bias: %f\n", *neuron.bias);
}

Tensor convNeuron_forward(ConvNeuron neuron, Tensor input) {
    Tensor output = tensor_init(input.depth, input.height / neuron.stride, input.width / neuron.stride);
    Tensor input = tensor_pad(input, neuron.kernel_size / 2);

    for (int depth = 0; depth < input.depth; depth++) {
        for (int height = 0; height < input.height; height += neuron.stride) {
            for (int width = 0; width < input.width; width += neuron.stride) {
                double sum = 0;
                for (int i = 0; i < neuron.kernel_size; i++) {
                    for (int j = 0; j < neuron.kernel_size; j++) {
                        sum += input.data[depth][height + i][width + j] * neuron.weights.data[0][i][j];
                    }
                }
                output.data[depth][height / neuron.stride][width / neuron.stride] = sum + *neuron.bias;
            }
        }
    }

    tensor_free(input);
    return output;
} 

/*-------------------------------------------------------------Linear Neuron------------------------------------------------------------------------*/

LinearNeuron linearNeuron_init(int input_size) {
    LinearNeuron neuron;
    neuron.input_size = input_size;
    neuron.weights = tensor_init(1, input_size, 1);
    tensor_random(neuron.weights);
    neuron.bias = (double*)malloc(sizeof(double));
    *neuron.bias = rand() / (double)RAND_MAX;
    return neuron;
}

void linearNeuron_free(LinearNeuron neuron) {
    tensor_free(neuron.weights);
    free(neuron.bias);
}

void linearNeuron_print(LinearNeuron neuron) {
    printf("input_size: %d\n", neuron.input_size);
    printf("weights:\n");
    tensor_print(neuron.weights);
    printf("bias: %f\n", *neuron.bias);
}

double linearNeuron_forward(LinearNeuron neuron, Tensor input) {
    double sum = 0;
    for (int i = 0; i < neuron.input_size; i++) {
        sum += input.data[0][i][0] * neuron.weights.data[0][i][0];
    }
    return sum + *neuron.bias;
}

/*-------------------------------------------------------------Batch Normalization------------------------------------------------------------------------*/

BatchNorm batchNorm_init(double epsilon) {
    BatchNorm batchNorm;
    batchNorm.epsilon = (double*)malloc(sizeof(double));
    *batchNorm.epsilon = epsilon;
    batchNorm.gamma = (double*)malloc(sizeof(double));
    *batchNorm.gamma = rand() / (double)RAND_MAX;
    batchNorm.beta = (double*)malloc(sizeof(double));
    *batchNorm.beta = rand() / (double)RAND_MAX;
    return batchNorm;
}

void batchNorm_free(BatchNorm batchnorm) {
    free(batchnorm.epsilon);
    free(batchnorm.gamma);
    free(batchnorm.beta);
}

void batchNorm_print(BatchNorm batchnorm) {
    printf("epsilon: %f\n", *batchnorm.epsilon);
    printf("gamma: %f\n", *batchnorm.gamma);
    printf("beta: %f\n", *batchnorm.beta);
}

Tensor batchNorm_forward (BatchNorm batchNorm, Tensor input) {
    Tensor output = tensor_init(input.depth, input.height, input.width);
    double sum = 0, sum_sq = 0, mean, variance;

    // Calculate mean
    for (int i = 0; i < input.depth; i++) {
        for (int j = 0; j < input.height; j++) {
            for (int k = 0; k < input.width; k++) {
                sum += input.data[i][j][k];
            }
        }
    }
    mean = sum / (input.depth * input.height * input.width);

    // Calculate variance
    for (int i = 0; i < input.depth; i++) {
        for (int j = 0; j < input.height; j++) {
            for (int k = 0; k < input.width; k++) {
                sum_sq += pow(input.data[i][j][k] - mean, 2);
            }
        }
    }
    variance = sum_sq / (input.depth * input.height * input.width);

    // Normalize
    for (int i = 0; i < input.depth; i++) {
        for (int j = 0; j < input.height; j++) {
            for (int k = 0; k < input.width; k++) {
                output.data[i][j][k] = (*batchNorm.gamma) * (input.data[i][j][k] - mean) / sqrt(variance + *batchNorm.epsilon) + *batchNorm.beta;
            }
        }
    }

    tensor_free(input);
    return output;
}

/*-------------------------------------------------------------Nonlinearity------------------------------------------------------------------------*/

Tensor ReLU(Tensor input) {
    Tensor output = tensor_init(input.depth, input.height, input.width);
    for (int i = 0; i < input.depth; i++) {
        for (int j = 0; j < input.height; j++) {
            for (int k = 0; k < input.width; k++) {
                output.data[i][j][k] = max(0, input.data[i][j][k]);
            }
        }
    }
    tensor_free(input);
    return output;
}

Tensor nonlinear(int nonlinearity_type, Tensor input) {
    Tensor output = tensor_init(input.depth, input.height, input.width);
    switch (nonlinearity_type) {
        {
        case 1:
            output = ReLU(input);
            break;
        default:
            break;
        }
    }
    tensor_free(input);
    return output;
}

/*-------------------------------------------------------------Convolutional Layer------------------------------------------------------------------------*/

ConvLayer convLayer_init(int layer_size, int kernel_size, int stride) {
    ConvLayer layer;
    layer.layer_size = layer_size;
    layer.kernel_size = kernel_size;
    layer.stride = stride;
    layer.convNeurons = (ConvNeuron*)malloc(layer_size * sizeof(ConvNeuron));
    for (int i = 0; i < layer_size; i++) {
        layer.convNeurons[i] = convNeuron_init(kernel_size, stride);
    }
    return layer;
}

void convLayer_free(ConvLayer layer) {
    for (int i = 0; i < layer.layer_size; i++) {
        convNeuron_free(layer.convNeurons[i]);
    }
    free(layer.convNeurons);
}

void convLayer_print(ConvLayer layer) {
    printf("layer_size: %d\n", layer.layer_size);
    printf("kernel_size: %d\n", layer.kernel_size);
    printf("stride: %d\n", layer.stride);
    printf("convNeurons:\n");
    for (int i = 0; i < layer.layer_size; i++) {
        printf("convNeuron %d:\n", i);
        convNeuron_print(layer.convNeurons[i]);
    }
}

Tensor convLayer_forward(ConvLayer layer, Tensor input) {
    Tensor output = tensor_init(layer.layer_size, input.height / layer.stride, input.width / layer.stride);
    Tensor input = tensor_pad(input, layer.kernel_size / 2);

    for (int i = 0; i < layer.layer_size; i++) {
        Tensor temp = convNeuron_forward(layer.convNeurons[i], input);
        tensor_embed(output, temp, i*input.depth);
    }

    tensor_free(input);
    return output;
}

/*-------------------------------------------------------------Linear Layer------------------------------------------------------------------------*/

LinearLayer linearLayer_init(int input_size, int layer_size) {
    LinearLayer layer;
    layer.input_size = input_size;
    layer.layer_size = layer_size;
    layer.linearNeurons = (LinearNeuron*)malloc(layer_size * sizeof(LinearNeuron));
    for (int i = 0; i < layer_size; i++) {
        layer.linearNeurons[i] = linearNeuron_init(input_size);
    }
    return layer;
}

void linearLayer_free(LinearLayer layer) {
    for (int i = 0; i < layer.layer_size; i++) {
        linearNeuron_free(layer.linearNeurons[i]);
    }
    free(layer.linearNeurons);
}

void linearLayer_print(LinearLayer layer) {
    printf("input_size: %d\n", layer.input_size);
    printf("layer_size: %d\n", layer.layer_size);
    printf("linearNeurons:\n");
    for (int i = 0; i < layer.layer_size; i++) {
        printf("linearNeuron %d:\n", i);
        linearNeuron_print(layer.linearNeurons[i]);
    }
}

Tensor linearLayer_forward(LinearLayer layer, Tensor input) {
    Tensor output = tensor_init(1, 1, layer.layer_size);
    for (int i = 0; i < layer.layer_size; i++) {
        output.data[0][0][i] = linearNeuron_forward(layer.linearNeurons[i], input);
    }
    tensor_free(input);
    return output;
}

/*-------------------------------------------------------------Convolutional Block------------------------------------------------------------------------*/

ConvBlock convBlock_init(int layer_size, int kernel_size, int stride, int nonlinearity_type) {
    ConvBlock block;
    block.convLayer = convLayer_init(layer_size, kernel_size, stride);
    block.batchNorm = batchNorm_init(1e-5);
    block.nonlinearity_type = nonlinearity_type;
    return block;
}

void convBlock_free(ConvBlock block) {
    convLayer_free(block.convLayer);
    batchNorm_free(block.batchNorm);
}

void convBlock_print(ConvBlock block) {
    printf("convLayer:\n");
    convLayer_print(block.convLayer);
    printf("batchNorm:\n");
    batchNorm_print(block.batchNorm);
    printf("nonlinearity_type: %d\n", block.nonlinearity_type);
}

Tensor convBlock_forward(ConvBlock block, Tensor input) {
    Tensor output = convLayer_forward(block.convLayer, input);
    output = batchNorm_forward(block.batchNorm, output);
    output = nonlinear(block.nonlinearity_type, output);
    return output;
}

/*-------------------------------------------------------------Linear Block------------------------------------------------------------------------*/

LinearBlock linearBlock_init(int input_size, int layer_size, int nonlinearity_type) {
    LinearBlock block;
    block.linearLayer = linearLayer_init(input_size, layer_size);
    block.batchNorm = batchNorm_init(1e-5);
    block.nonlinearity_type = nonlinearity_type;
    return block;
}

void linearBlock_free(LinearBlock block) {
    linearLayer_free(block.linearLayer);
    batchNorm_free(block.batchNorm);
}

void linearBlock_print(LinearBlock block) {
    printf("linearLayer:\n");
    linearLayer_print(block.linearLayer);
    printf("batchNorm:\n");
    batchNorm_print(block.batchNorm);
    printf("nonlinearity_type: %d\n", block.nonlinearity_type);
}

Tensor linearBlock_forward(LinearBlock block, Tensor input) {
    Tensor output = linearLayer_forward(block.linearLayer, input);
    output = batchNorm_forward(block.batchNorm, output);
    output = nonlinear(block.nonlinearity_type, output);
    return output;
}

/*-------------------------------------------------------------Residual Block------------------------------------------------------------------------*/

ResBlock resBlock_init(int num_convLayers, int* layer_size, int* kernel_size, int* stride, int* nonlinearity_type) {
    ResBlock block;
    block.num_convLayers = num_convLayers;
    for (int i = 0; i < num_convLayers-1; i++) {
        block.convBlock[i] = convBlock_init(layer_size[i], kernel_size[i], stride[i], nonlinearity_type[i]);
    }
    block.convLayer = convLayer_init(layer_size[num_convLayers-1], kernel_size[num_convLayers-1], stride[num_convLayers-1]);
    block.batchNorm = batchNorm_init(1e-5);
    block.nonlinearity_type = nonlinearity_type[num_convLayers-1];
    return block;
}

void resBlock_free(ResBlock block) {
    for (int i = 0; i < block.num_convLayers-1; i++) {
        convBlock_free(block.convBlock[i]);
    }
    convLayer_free(block.convLayer);
    batchNorm_free(block.batchNorm);
}

void resBlock_print(ResBlock block) {
    printf("num_convLayers: %d\n", block.num_convLayers);
    printf("convBlocks:\n");
    for (int i = 0; i < block.num_convLayers-1; i++) {
        printf("convBlock %d:\n", i);
        convBlock_print(block.convBlock[i]);
    }
    printf("convLayer:\n");
    convLayer_print(block.convLayer);
    printf("batchNorm:\n");
    batchNorm_print(block.batchNorm);
    printf("nonlinearity_type: %d\n", block.nonlinearity_type);
}

Tensor resBlock_forward(ResBlock block, Tensor input) {
    Tensor output = input;
    for (int i = 0; i < block.num_convLayers-1; i++) {
        output = convBlock_forward(block.convBlock[i], output);
    }
    output = convLayer_forward(block.convLayer, output);
    output = batchNorm_forward(block.batchNorm, output);
    output = nonlinear(block.nonlinearity_type, output);
    output = tensor_concatenate(input, output);
    return output;
}