#pragma once
#include <cmath>
#include <iostream>
#include "structures.hpp"
#include "transformers.hpp"
#include "ray.hpp"

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
    Cylinder(Point direction, Point center, double radius, double height) : direction(direction), center(center), radius(radius), height(height) {}
    bool Intersect(Ray &ray, double *results) override;
};