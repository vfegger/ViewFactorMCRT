#pragma once
#include <cmath>
#include <random>

class MonteCarlo
{
private:
    long long unsigned seed;

public:
    MonteCarlo(long long unsigned seed) : seed(seed) {}
    double Simulate(bool (*f)(std::default_random_engine &), long long unsigned length);
};