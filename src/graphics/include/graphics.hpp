#pragma once
#include "structures.hpp"
#include "transformers.hpp"
#include "ray.hpp"
#include <cmath>

class Ray;

class Mesh
{
public:
    virtual bool Intersect(Ray &ray, double *results) = 0;
};

class Cylinder : public Mesh
{
protected:
    Point direction;
    Point center;
    double radius;
    double height;

public:
    Cylinder(Point direction, Point center, double radius, double height);
    bool Intersect(Ray &ray, double *results) override;
};