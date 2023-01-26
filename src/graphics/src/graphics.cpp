#include "../include/graphics.hpp"

bool Cylinder::Intersect(Ray &ray, double *results)
{
    Rotator rotator;
    Point dirRotation = Point(0,0,1) & direction;
    Point rayOrigin = ray.origin - center;
    Point rayDirection = ray.direction;
    double angleS = std::asin(std::sqrt(dirRotation|dirRotation)/sqrt(direction|direction));
    double angleC = std::acos(direction.z/std::sqrt(direction|direction));
    double angle = ((angleS > 0) - (angleS < 0)) * angleC;
    
    rotator.SetTransformation(dirRotation, angle);
    rayOrigin = rotator.Transform(rayOrigin);
    rayDirection = rotator.Transform(rayDirection);

    double a = rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y;
    double b = 2.0 * (rayOrigin.x * rayDirection.x + rayOrigin.y * rayDirection.y);
    double c = rayOrigin.x * rayOrigin.x + rayOrigin.y * rayOrigin.y - radius * radius;

    //std::cout << "\t" << a << " " << b << " " << c << "\n";

    // Solves Quadratic Equation
    double tol = 0.0001;
    double t1 = 0.0;
    double t2 = 0.0;
    bool hit = true;
    if (std::abs(a) < tol)
    {
        if (std::abs(b) > tol)
        {
            t1 = t2 = -c / b;
        }
        else
        {
            if (std::abs(c) > tol)
            {
                hit = false;
            }
        }
    }
    else
    {
        double delta = b * b - 4.0 * a * c;
        //std::cout << delta << "\n";
        if (delta < -tol)
        {
            hit = false;
        }
        else if (std::abs(delta) < tol)
        {
            t1 = t2 = -b / (2 * a);
        }
        else
        {
            double temp = std::sqrt(delta);
            t1 = (-b - temp) / (2 * a);
            t2 = (-b + temp) / (2 * a);
        }
    }
    
    double partial1 = rayOrigin.z + t1 * rayDirection.z;
    double partial2 = rayOrigin.z + t2 * rayDirection.z;
    double ref1 = ((rayOrigin.z > 0) - (rayOrigin.z < 0)) * height;
    double ref2 = ((rayOrigin.z > 0) - (rayOrigin.z < 0)) * height;
    //std::cout << "\t" << t1 << "\n";
    //std::cout << "\t" << partial1 << "\n";
    //std::cout << "\t" << t2 << "\n";
    //std::cout << "\t" << partial2 << "\n";

    if (std::abs(partial1) > height && std::abs(rayDirection.z) > 0.0)
    {
        t1 = - (ref1 - partial1) / rayDirection.z;
    }
    if (std::abs(partial2) > height && std::abs(rayDirection.z) > 0.0)
    {
        t2 = - (ref2 - partial2) / rayDirection.z;
    }

    if (t1 >= 0)
    {
        *results = t1;
    }
    else if (t2 >= 0)
    {
        *results = t2;
    }
    else
    {
        *results = 0.0;
        hit = false;
    }
    return hit;
}