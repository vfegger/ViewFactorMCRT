#pragma once
#include "structures.hpp"
#include <cmath>
#include <iostream>

class Rotator
{
private:
    Quaternion transformation;

public:
    Quaternion SetTransformation(Point direction, double angle)
    {
        angle /= 2.0;
        double c = cos(angle);
        double s = sin(angle);
        direction *= (1.0 / sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z));
        transformation = Quaternion(c, s * direction.x, s * direction.y, s * direction.z);
        std::cout << "Quarternion Quat:" << transformation.r << " " << transformation.i << " " << transformation.j << " " << transformation.k << "\n\n";
        return transformation;
    }
    Point Transform(Point point) {
        Quaternion aux = Quaternion(point.x, point.y, point.z);
        std::cout << "Quarternion Bfr:" << aux.r << " " << aux.i << " " << aux.j << " " << aux.k << "\n";
        aux = transformation * aux * (!transformation);
        std::cout << "Quarternion Aft:" << aux.r << " " << aux.i << " " << aux.j << " " << aux.k << "\n\n";

        return Point(aux.i, aux.j, aux.k);
    }
};
