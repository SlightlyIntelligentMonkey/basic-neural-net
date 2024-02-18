#include <cstdio>
#include <cstring>
#include "network.h"
#include "mnist/mnist_data.h"

const char *trainingLabels = "train-labels.idx1-ubyte";
const char *trainingImages = "train-images.idx3-ubyte";

const char *testingLabels = "t10k-labels.idx1-ubyte";
const char *testingImages = "t10k-images.idx3-ubyte";

int main(int argc, char **argv)
{
    bool save = false; int snArg = -1;
    bool load = false; int lnArg = -1;
    unsigned int epochs = 1000;
    for(int i = 0; i < argc; i++)
    {
        if(!strcmp(argv[i], "-s"))
        {
            save = true;
            snArg = i+1;
        }
        if(!strcmp(argv[i], "-l"))
        {
            load = true;
            lnArg = i+1;
        }
        if(!strcmp(argv[i], "-e"))
        {
            sscanf(argv[i+1], "%u", &epochs);
        }
    }

    printf("Initializing network...\n");
    //initialize the network
    //Network *net = new Network(4, n, 0.5);
    unsigned int n[4] = {28*28, 32, 16, 10};
    Network *net = nullptr;
    if(load == true)
        net = new Network(argv[lnArg], 0.5);
    else
        net = new Network(4, n, 0.5);

    //load the training data
    printf("Loading training data...\n");
    LABELSET labset;
    IMAGESET imgset;
    loadDatabase(&labset, testingLabels);
    loadDatabase(&imgset, testingImages);

    //puts the data in a format the neural network's functions can understand
    //printf("Parsing training data...\n");
    printf("Parsing testing data...\n");
    double **imgdata = new double* [imgset.count];
    double **labdata = new double* [labset.count];
    for(unsigned int i = 0; i < labset.count; i++)
    {
        unsigned int imgsize = imgset.pRows*imgset.pCols;
        imgdata[i] = new double [imgsize];
        for(unsigned int j = 0; j < imgsize; j++)
            imgdata[i][j] = (double)imgset.data[(i*imgsize)+j]/255.0f;

        unsigned int num = imgset.data[(i*imgsize)];
        labdata[i] = new double [10];
        for(unsigned int j = 0; j < 10; j++)
        {
            if(j == num)
                labdata[i][j] = 1.0f;
            else
                labdata[i][j] = 0.0f;
        }
    }

    //test the network
    for(unsigned int i = 0; i < epochs; i++)
    {
        net->Test(5, labset.count, imgdata, labdata);
    }

    //train the network
    //printf("Training...\n");
    //for(unsigned int i = 0; i < epochs; i++)
    //{
    //    net->Train(5, labset.count, imgdata, labdata);
    //}

    if(save == true)
        net->SaveNetwork(argv[snArg]);

    return 0;
}