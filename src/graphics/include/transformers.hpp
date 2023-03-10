#pragma once
#include <cmath>
#include <iostream>
#include "structures.hpp"

class Rotator
{
private:
    Quaternion transformation;

public:
    Quaternion SetTransformation(Point direction, double angle)
    {
        double temp = direction | direction;
        if (temp == 0.0)
        {
            transformation = Quaternion(1.0, 0.0, 0.0, 0.0);
        }
        else
        {
            angle /= 2.0;
            double c = cos(angle);
            double s = sin(angle);
            direction *= (1.0 / sqrt(temp));
            transformation = Quaternion(c, s * direction.x, s * direction.y, s * direction.z);
        }
        return transformation;
    }
    Point Transform(Point point)
    {
        Quaternion aux = Quaternion(point.x, point.y, point.z);
        aux = transformation * aux * (!transformation);

        return Point(aux.i, aux.j, aux.k);
    }
};
