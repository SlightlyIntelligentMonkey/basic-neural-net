#include "network.h"
#include <cstring>
#include <fstream>

Network::Network(unsigned int l, unsigned int *n, double rate) : layers(l), L(n), y(rate)
{
    net = new Neuron* [layers];
    net[0] = new Neuron [L[0]];
    for(unsigned int i = 1; i < layers; i++)
    {
        net[i] = new Neuron [L[i]];
        for(unsigned int j = 0; j < L[i]; j++)
        {
            net[i][j].w = new double [L[i-1]];
            for(unsigned int k = 0; k < L[i-1]; k++)
                net[i][j].w[k] = randomDouble(0.0, 1.0);
        }
    }

    //allocate space for storing the error for backpropogation
    error = new double** [layers];
    error[0] = new double* [L[0]];
    for(unsigned int i = 1; i < layers; i++)
    {
        error[i] = new double* [L[i]];
        for(unsigned int j = 0; j < L[i]; j++)
            error[i][j] = new double[L[i-1]];
    }

    return;
}

Network::Network(const char *path, double rate) : y(rate)
{
    LoadNetwork(path);

    //allocate space for storing the error for backpropogation
    error = new double** [layers];
    error[0] = new double* [L[0]];
    for(unsigned int i = 1; i < layers; i++)
    {
        error[i] = new double* [L[i]];
        for(unsigned int j = 0; j < L[i]; j++)
            error[i][j] = new double[L[i-1]];
    }
    return;
}

Network::~Network()
{
    for(unsigned int i = 1; i < layers; i++)
    {
        for(unsigned int j = 0; layers < L[i]; j++)
        {
            delete[] net[i][j].w;
            delete[] error[i][j];
        }
    }
    for(unsigned int i = 0; i < layers; i++)
    {
        delete[] net[i];
        delete[] error[i];
    }
    delete[] net;
    delete[] error;

    return;
}

//simplest method for a neural network
double *Network::ForwardPropagation(double *input)
{
    for(unsigned int i = 0; i < L[0]; i++)
    {
        net[0][i].a = input[i];
    }

    //calculate the activations layer by layer
    for(unsigned int i = 1; i < layers; i++)
    {
        for(unsigned int j = 0; j < L[i]; j++)
        {
            net[i][j].z = 0;
            for(unsigned int k = 0; k < L[i-1]; k++)
            {
                net[i][j].z += net[i-1][k].a*net[i][j].w[k];
            }
            net[i][j].z += net[i][j].b;

            net[i][j].a = sigmoid(net[i][j].z);
        }
    }

    //copy the activation of the last layer into an array and return that
    double *result = new double[L[layers-1]];
    for(unsigned int i = 0; i < L[layers-1]; i++)
        result[i] = net[layers-1][i].a;

    return result;
}

void Network::BackPropagation(double *output)
{
    //calculate the error
    for(unsigned int j = 0; j < L[layers-1]; j++)
    {   //calculates the error in the output layer
        error[layers-1][j][0] = (net[layers-1][j].a - output[j])*dsigmoid(net[layers-1][j].z);
    }
    //backpropagate the output layer error
    for(unsigned int i = layers-2; i > 1; i--)
    {
        for(unsigned int j = 0; j < L[i]; j++)
        {
            double dactivation = dsigmoid(net[i][j].z);
            for(unsigned int k = 0; k < L[i+1]; k++)
            {
                for(unsigned int l = 0; l < L[i+1]; l++)
                {
                    error[i][j][k] += net[i][j].w[k]*error[i+1][k][l];
                }
                error[i][j][k] *= dactivation;
            }
        }
    }

    //stochastic gradient descent
    for(unsigned int i = 1; i < layers; i++)
    {
        for(unsigned int j = 0; j < L[i]; j++)
        {
            for(unsigned int k = 0; k < L[i-1]; k++)
            {
                net[i][j].w[k] -= y*error[i][j][k]*net[i-1][k].a;
                net[i][j].b -= y*error[i][j][k];
            }
        }
    }

    return;
}

void Network::Train(int eplen, unsigned int numdata, double **inputs, double **outputs)
{
    double avgcost = 0;
    for(unsigned int i = 0; i < eplen; i++)
    {
        unsigned int indice = randomUInt(0, numdata-1);
        double *actual = ForwardPropagation(inputs[indice]);
        BackPropagation(outputs[indice]);
        avgcost += QuadraticCost(actual, outputs[indice]);
    }

    avgcost /= eplen;
    printf("The average is %lf\n", avgcost);

    return;
}

void Network::Test(int eplen, unsigned int numdata, double **inputs, double **outputs)
{
    double avgcost = 0;
    for(unsigned int i = 0; i < eplen; i++)
    {
        unsigned int indice = randomUInt(0, numdata-1);
        double *actual = ForwardPropagation(inputs[indice]);
        avgcost += QuadraticCost(actual, outputs[indice]);
    }

    avgcost /= eplen;
    printf("The average is %lf\n", avgcost);

    return;
}

double Network::QuadraticCost(double *actual, double *desired)
{
    double cost = 0;
    for(unsigned int i = 0; i < L[layers-1]; i++)
    {
        double diff = actual[i] - desired[i];
        cost += diff*diff;
    }
    return cost/(2*layers);
}

//double Network::Cost(double *actual, double *desired)
//{
//    double cost;
//    for(unsigned int i = 0; i < L[layers-1]; i++)
//    {
//        cost += actual[i] - desired[i];
//    }
//    return cost;
//}

void Network::LoadNetwork(const char *name)
{
    char filename[256];
    strcpy(filename, name);
    strcat(filename, ".nn");
    FILE *fp = fopen(filename, "rb");

    fread(&layers, sizeof(unsigned int), 1, fp);

    L = new unsigned int[layers];
    net = new Neuron* [layers];

    for(unsigned int i = 0; i < layers; i++)
    {
        fread(&L[i], sizeof(unsigned int), 1, fp);
        net[i] = new Neuron [L[i]];
    }

    for(unsigned int i = 1; i < layers; i++)
    {
        for(unsigned int j = 0; j < L[i]; j++)
        {
            fread(&net[i][j].b, sizeof(double), 1, fp);
            net[i][j].w = new double[L[i-1]];
            fread(net[i][j].w, sizeof(double), L[i-1], fp);
        }
    }

    return;
}

void Network::SaveNetwork(const char *name)
{
    char filename[256];
    strcpy(filename, name);
    strcat(filename, ".nn");
    FILE *fp = fopen(filename, "wb");
    
    fwrite(&layers, sizeof(unsigned int), 1, fp);

    for(unsigned int i = 0; i < layers; i++)
        fwrite(&L[i], sizeof(unsigned int), 1, fp);

    for(unsigned int i = 1; i < layers; i++)
    {
        for(unsigned int j = 0; j < L[i]; j++)
        {
            fwrite(&net[i][j].b, sizeof(double), 1, fp);
            fwrite(net[i][j].w, sizeof(double), L[i-1], fp);
        }
    }
    
    return;
}