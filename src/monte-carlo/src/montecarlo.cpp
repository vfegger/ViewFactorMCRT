#include "../include/montecarlo.hpp"

double MonteCarlo::Simulate(bool (*f)(std::default_random_engine &), long long unsigned length)
{
    std::default_random_engine generator(seed);
    long long unsigned acc = 0u;
    for (long long unsigned i = 0llu; i < length; i++)
    {
        acc += (f(generator)) ? 1llu : 0llu;
    }
    return ((double) acc) / ((double) length);
}