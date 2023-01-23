#pragma once
#include <cmath>

class MonteCarlo
{
private:
    long long unsigned seed;

public:
    MonteCarlo(long long unsigned seed) : seed(seed) {}
    void Calculate(bool (*f)(double));
};