#pragma once
#include <cmath>
#include <random>
#include <functional>

class MonteCarlo
{
private:
    long long unsigned seed;

public:
    MonteCarlo(long long unsigned seed) : seed(seed) {}
    double Simulate(std::function<bool(std::default_random_engine &)> f, long long unsigned length);
};