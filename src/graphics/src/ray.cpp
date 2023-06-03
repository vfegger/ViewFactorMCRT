#include "../include/ray.hpp"

bool Ray::Intersect(Mesh *meshes, unsigned length, double& result)
{
    double *results = new double[length];
    double value = 0.0;
    unsigned index = 0u;
    bool hit = false;
    for (unsigned i = 0u; i < length; i++)
    {
        bool aux = meshes[i].Intersect(*this, results + i);
        if (aux){
            if(hit) {
                if(results[i] < value) {
                    index = i;
                    value = results[i];
                }
            } else {
                index = i;
                value = results[i];
                hit = true;
            }
        }
    }
    result = value;
    delete[] results;
    return hit;
}