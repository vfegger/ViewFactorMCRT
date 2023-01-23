#pragma once
#include "structures.hpp"
#include "transformers.hpp"
#include "../../ray-tracing/include/ray.hpp"
#include <cmath>

class Mesh
{
public:
    virtual bool Intersect(Ray &ray, double *results) = 0;
};

class Cylinder : Mesh
{
protected:
    Point direction;
    Point center;
    double radius;
    double height;

public:
    bool Intersect(Ray &ray, double *results) override;
};