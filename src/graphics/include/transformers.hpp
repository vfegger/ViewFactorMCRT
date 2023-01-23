#pragma once
#include "structures.hpp"
#include <cmath>

class Rotator
{
private:
    Quartenion transformation;

public:
    Quartenion SetTransformation(Point direction, double angle)
    {
        angle /= 2.0;
        double c = cos(angle);
        double s = sin(angle);
        direction *= (1.0 / sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z));
        transformation = Quartenion(c, s * direction.x, s * direction.y, s * direction.z);
        return transformation;
    }
    Point Transform(Point point) {
        Quartenion aux = Quartenion(point.x, point.y, point.z);
        aux = transformation * aux * (!transformation);
        return Point(aux.i, aux.j, aux.k);
    }
};
