#include <math.h>

#define e 2.71828182845904523536028747135266249775724709369995

inline double sigmoid(double input)
{
    return 1/(1+exp(-input));
}

inline double dsigmoid(double input) //derivative of the sigmoid function
{
    double sig = sigmoid(input);
    return sig*(1-sig);
}

inline int randomInt(int min, int max)
{
    return min + rand()%(max-min);
}

inline unsigned int randomUInt(unsigned int min, unsigned int max)
{
    return min + static_cast<unsigned int>(rand())%(max-min);
}

inline double randomDouble(double min, double max)
{
    return min + ((static_cast<double>(rand()) / static_cast<double>(RAND_MAX))*(max-min));
}