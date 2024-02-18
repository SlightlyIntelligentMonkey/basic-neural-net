#include "mathfunctions.h"

struct Neuron
{
    double b;
    double *w;
    double z;
    double a;
};

typedef double ActivationFunction(double input);

class Network
{
private:
    unsigned int layers;    //number of layers in the netork (include input and output)
    unsigned int *L;        //number of neurons per layer
    double y;               //learning constant

    Neuron **net;
    //double **b;
    //double ***w;
    //double **z;
    //double **a;

    double ***error;
    
public:
    Network(unsigned int l, unsigned int *n, double rate); //l = number of layers, *n is an array of the number of neurons per layer, rate is the learning rate
    Network(const char *path, double rate);
    ~Network();

    double *ForwardPropagation(double *input);
    void BackPropagation(double *output);

    void Train(int epochs, unsigned int numdata, double **inputs, double **outputs);
    void Test(int eplen, unsigned int numdata, double **inputs, double **outputs);

    //double Cost(double *actual, double *desired);
    double QuadraticCost(double *input, double *desired);

    void LoadNetwork(const char *name);
    void SaveNetwork(const char *name);
};