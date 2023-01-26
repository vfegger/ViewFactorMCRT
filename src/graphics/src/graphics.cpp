#include "../include/graphics.hpp"

bool Cylinder::Intersect(Ray &ray, double *results)
{
    Rotator rotator;
    Point dirRotation = Point(0,0,1) & direction;
    Point rayOrigin = ray.origin - center;
    Point rayDirection = ray.direction;
    double angleS = asin(sqrt(dirRotation|dirRotation)/sqrt(direction|direction));
    double angleC = acos(direction.z/sqrt(direction|direction));
    double angle = ((angleS > 0) - (angleS < 0)) * angleC;
    
    std::cout << "\t" << angle << "\n";
    rotator.SetTransformation(dirRotation, angle);
    rayOrigin = rotator.Transform(rayOrigin);
    rayDirection = rotator.Transform(rayDirection);
    std::cout << "\tNormal: " << ray.origin.x << " " << ray.origin.y << " " << ray.origin.z << "\n";
    std::cout << "\tNormal: " << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << "\n";
    std::cout << "\tTransf: " << rayOrigin.x << " " << rayOrigin.y << " " << rayOrigin.z << "\n";
    std::cout << "\tTransf: " << rayDirection.x << " " << rayDirection.y << " " << rayDirection.z << "\n";

    double a = rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y;
    double b = 2.0 * (rayOrigin.x * rayDirection.x + rayOrigin.y * rayDirection.y);
    double c = rayOrigin.x * rayOrigin.x + rayOrigin.y * rayOrigin.y - radius * radius;

    std::cout << "\t" << a << " " << b << " " << c << "\n";

    // Solves Quadratic Equation
    double tol = 0.0001;
    double t1 = 0.0;
    double t2 = 0.0;
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
        }
        else
        {
            double temp = sqrt(delta);
            t1 = (-b - temp) / (2 * a);
            t2 = (-b + temp) / (2 * a);
        }
    }
    
    double partial1 = rayOrigin.z + t1 * rayDirection.z;
    double partial2 = rayOrigin.z + t2 * rayDirection.z;

    if (abs(partial1) > height)
    {
        t1 = (height - partial1) / rayDirection.z;
    }
    if (abs(partial2) > height)
    {
        t2 = (height - partial2) / rayDirection.z;
    }

    if (t1 >= 0)
    {
        *results = partial1;
    }
    else if (t2 >= 0)
    {
        *results = partial2;
    }
    else
    {
        *results = 0.0;
        hit = false;
    }
    return hit;
}