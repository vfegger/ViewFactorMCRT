#include "../include/montecarlo.hpp"

double MonteCarlo::Simulate(std::function<bool(std::default_random_engine &)> f, long long unsigned length)
{
    std::cout << "Simulation Start:\n";
    std::default_random_engine generator(seed);
    long long unsigned acc = 0u;
    long long unsigned percent = (length > 100llu) ? 100llu : length; 
    for (long long unsigned i = 0llu; i < length; i++)
    {
        if (i % (length / percent) == 0)
        {
            long long unsigned temp = i / (length / percent);
            std::cout << "[";
            for (long long unsigned j = 0llu; j < 100llu; j++)
            {
                if (j < temp)
                {
                    std::cout << "=";
                }
                else if (j == temp)
                {
                    std::cout << ">";
                }
                else
                {
                    std::cout << " ";
                }
            }
            std::cout << "] " << temp << " %\r";
            std::cout.flush();
        }
        acc += (f(generator)) ? 1llu : 0llu;
    }
    std::cout << "\n";
    return ((double)acc) / ((double)length);
}