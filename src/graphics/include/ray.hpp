#pragma once
#include "graphics.hpp"

class Mesh;

class Ray
{
private:
public:
    Point origin;
    Point direction;
    Ray(Point origin, Point direction) : origin(origin), direction(direction) {}
    bool Intersect(Mesh *meshes, unsigned length, double &result);
};