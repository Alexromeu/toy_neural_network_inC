#include <stdio.h>
#include <stdlib.h>

#define INPUT 50 
#define HIDDEN 20 
#define OUTPUT 10

//OJO: weights are associated with connections
typedef struct __neuron_t {
    float value;
    float bias;
    float *weights;
} neuron_t;

typedef struct __layer_t {
    int count;
    neuron_t *neurons;   //elements here go after passed to sigmoid... middle layer
} layer_t;  

typedef struct __neural_network_t {
    int num_layers;
    layer_t *layers;
} neural_network_t;

void initialize_network(neural_network_t *network) {
    int total_layers = network->num_layers;
    layer_t *layers = malloc(total_layers * sizeof(layer_t));
    
}


int main(void) {


    return 0;
}