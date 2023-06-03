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

    // Subdivisions
    unsigned Lr, Lth, Lz;

    long long unsigned collisionCount;
    long long unsigned *collision;

public:
    Cylinder(Point direction, Point center, double radius, double height) : direction(direction), center(center), radius(radius), height(height), Lr(0u), Lth(0u), Lz(0u), collisionCount(0u), collision(NULL) {}
    bool Intersect(Ray &ray, double *results) override;
    void SetupCollision(unsigned Lr, unsigned Lth, unsigned Lz);
    long long unsigned GetCollision();
    long long unsigned GetCollision(unsigned i, unsigned j, unsigned k);
    void DeleteCollision();
};

class Quad : public Mesh
{
protected:
    Point center;
    Point normal;
    double height;
    double width;

    unsigned Lx, Ly;

    long long unsigned collisionCount;
    long long unsigned *collision;

public:
    Quad(Point center, Point normal, double height, double width) : center(center), normal(normal), height(height), width(width), collision(NULL) {}
    bool Intersect(Ray &ray, double *results) override;
    void SetupCollision(unsigned Lx, unsigned Ly);
    long long unsigned GetCollision();
    long long unsigned GetCollision(unsigned i, unsigned j);
    void DeleteCollision(); 
};