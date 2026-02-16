#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define MAX_WEIGHTS 20
#define MAX_NEURONS_PER_LAY 20
#define TOTAL_LAYERS 4 
#define INPUT_LAYER_MAXVAL 10
#define OUTPUT_LAYER_MAXVAL 5


//OJO: weights are associated with connections
typedef struct __neuron_t {
    float value;
    float bias;
    float weights[MAX_WEIGHTS];
} neuron_t;

typedef struct __input_t {
    float input_values[MAX_NEURONS_PER_LAY];
} input_t;

typedef struct __layer_t {
    int count;
    neuron_t *neurons;   //elements here go after passed to sigmoid... middle layer
} layer_t;  

typedef struct __neural_network_t {
    int num_layers;
    layer_t *layers;
    layer_t *output_layer;
} neural_network_t;

float random_num(float min, float max) {
    time_t t = time(NULL);
    int seed = (int)t;
    srand(seed);

    float scale = rand() / (float)RAND_MAX;
    return min + scale * (max - min);
}

float xavier_init(int n_in, int n_out) { 
    float limit = sqrtf(6.0f / (n_in + n_out)); 
    return random_num(-limit, limit); 
}

float sigmoid(float x) {
    return 1.0f / (1.0f + expf(-x));
}

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
                
                if (i > 0) {
                    int prev_layer_size = network->layers[i-1].count;
                    int current_layer_size = network->layers[i].count;
                    n->weights[k] = xavier_init(prev_layer_size, current_layer_size);
                }
                
            }
        }
    }

    network->output_layer = &network->layers[network->num_layers - 1];
    network->output_layer->count = OUTPUT_LAYER_MAXVAL;
    network->output_layer->neurons = realloc(network->output_layer->neurons, OUTPUT_LAYER_MAXVAL * sizeof(neuron_t));

    //set weights for output weights
    for (int i = 0; i < network->output_layer->count; i++) {
        for (int w = 0; w < MAX_WEIGHTS; w++) {
            network->output_layer->neurons[i].weights[w] = 0.3;//NEED RANDOM FUNCTION FOR THIS randnum(seed)
        }
    }

    for (int i = 0; i < network->num_layers; i++) { 
        for (int j = 0; j < network->layers[i].count; j++) {    
            for (int k = 0; k < MAX_WEIGHTS; k++) {
                count++;
                //printf("%f\n", network->layers[i].neurons[j].weights[k]);
            }    
        }
    }

    //network so far initialize with #2 to test
    //next we need to add a random nums function to set the values random
}

float feed_neuron(neuron_t *neuron, float *input_layer, int input_count) {
    int i = 0;
    float result = 0.0f;

    while (i < input_count) {
        result += neuron->weights[i] * input_layer[i]; 
        i++;
    }

    return result + neuron->bias; //res to pass to next layer neuronS
}



//this process layer 1 and pass data to layer 2 editing layer 2
//same process will repeat in layer 3 inheriting data from 2
//then need to deal with the output
void layer_processing(layer_t *in_layer, layer_t *out_layer) {
    float input_values[MAX_NEURONS_PER_LAY];
    for (int i = 0; i < in_layer->count; i++) {
        input_values[i] = in_layer->neurons[i].value;
    }

    for (int lo = 0; lo < out_layer->count; lo++) {
        float raw_sum = feed_neuron(&out_layer->neurons[lo], input_values, in_layer->count);
        out_layer->neurons[lo].value = sigmoid(raw_sum);
    }
    
}


int main(void) {
    neural_network_t network;
    network.num_layers = TOTAL_LAYERS;
    initialize_network(&network);

    layer_t *layer1 = &(network.layers[0]);
    layer_t *layer2 = &(network.layers[1]);
    layer_t *layer3 = &(network.layers[2]);
    layer_t *output_layer = network.output_layer;

    layer_processing(layer1, layer2);
    layer_processing(layer2, layer3);
    layer_processing(layer3, output_layer);

    for (int i = 0; i < output_layer->count; i++) {
        printf("%f\n", output_layer->neurons[i].value);
    }
    float r = random_num(3,2);
    printf("%f\n", r);
    return 0;
}