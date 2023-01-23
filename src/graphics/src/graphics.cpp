#include "../include/graphics.hpp"

bool Cylinder::Intersect(Ray &ray, double *results)
{
    Rotator rotator;
    Point direction;
    double angle;
    Point rayOrigin = ray.origin - center;
    Point rayDirection = ray.direction - center;
    rotator.SetTransformation(direction, angle);
    Point rayOrigin = rotator.Transform(ray.origin);
    Point rayDirection = rotator.Transform(ray.direction);

    double a = rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y;
    double b = 2.0 * (rayOrigin.x * rayDirection.x + rayOrigin.y * rayDirection.y);
    double c = rayOrigin.x * rayOrigin.x + rayOrigin.y * rayOrigin.y - radius * radius;

    // Solves Quadratic Equation
    double tol = 0.0001;
    double t1, t2;
    bool hit = true;
    if (abs(a) < tol)
    {
        if (abs(b) > tol)
        {
            t1 = t2 = -c / b;
        }
        else
        {
            if (abs(c) < tol)
            {
                t1 = t2 = 0.0;
            }
            else
            {
                hit = false;
            }
        }
    }
    else
    {
        double delta = b * b - 4.0 * a * c;
        if (delta < -tol)
        {
            hit = false;
        }
        else if (abs(delta) < tol)
        {
            t1 = t2 = -b / (2 * a);
        } else {
            double temp = sqrt(delta);
            t1 = (-b - temp) / (2*a);
            t2 = (-b + temp) / (2*a);
        }
    }

    double partial1 = rayOrigin.z + t1 * rayDirection.z;
    double partial2 = rayOrigin.z + t2 * rayDirection.z;

    if(abs(partial1) > height) {
        t1 = (height - partial1) / rayDirection.z;
    }
    if(abs(partial2) > height) {
        t2 = (height - partial2) / rayDirection.z;
    }

    if(t1 >= 0) {
        *results = partial1;
    } else if(t2 >= 0){
        *results = partial2;
    } else {
        *results = 0.0;
        hit = false;
    }
    return hit;
}