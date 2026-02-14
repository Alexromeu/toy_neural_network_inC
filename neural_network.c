#include <stdio.h>
#include <stdlib.h>

#define MAX_WEIGHTS 10
#define MAX_NEURONS_PER_LAY 10
#define TOTAL_LAYERS 10 

//OJO: weights are associated with connections
typedef struct __neuron_t {
    float value;
    float bias;
    float weights[MAX_WEIGHTS];
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
    int count = 0;
    network->layers = malloc(network->num_layers * sizeof(layer_t));

    for (int i = 0; i < network->num_layers; i++) {
        layer_t *layer = &network->layers[i];
        layer->count = MAX_NEURONS_PER_LAY;
        layer->neurons = malloc(layer->count * sizeof(neuron_t));

        for (int j = 0; j < layer->count; j++) {
            neuron_t *n = &layer->neurons[j];

            n->value = 0;
            n->bias = 0;

            for (int k = 0; k < MAX_WEIGHTS; k++) {
                n->weights[k] = 2;
            }
        }
    }
   
    for (int i = 0; i < network->num_layers; i++) { 
        for (int j = 0; j < network->layers[i].count; j++) {    
            for (int k = 0; k < MAX_WEIGHTS; k++) {
                count++;
                printf("%f\n", network->layers[i].neurons[j].weights[k]);
            }    
        }
    }
printf("neurons count-> %d\n", count);
    //network so far initialize with #2 to test
    //next we need to add a random nums function to set the values random
    // and set a function to feed input to first layer
}


int main(void) {
    neural_network_t network;
    network.num_layers = TOTAL_LAYERS;
    initialize_network(&network);

    return 0;
}