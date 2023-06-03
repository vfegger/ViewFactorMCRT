#include "../include/graphics.hpp"

bool solveq(double a, double b, double c, double &t1, double &t2, double tol = 0.0001)
{
    double delta = b * b - 4 * a * c;
    if (delta < 0.0)
    {
        return false;
    }
    else
    {
        delta = std::sqrt(delta);
        t1 = (-b + delta) / (2.0 * a);
        t2 = (-b - delta) / (2.0 * a);
        if (t2 < t1)
        {
            delta = t2;
            t2 = t1;
            t1 = delta;
        }
        return true;
    }
}

bool Cylinder::Intersect(Ray &ray, double *results)
{
    Rotator rotator;
    Point dirRotation = Point(0, 0, 1) & direction;
    Point rayOrigin = ray.origin - center;
    Point rayDirection = ray.direction;
    double angleS = std::asin(std::sqrt(dirRotation | dirRotation) / sqrt(direction | direction));
    double angleC = std::acos(direction.z / std::sqrt(direction | direction));
    double angle = ((angleS > 0) - (angleS < 0)) * angleC;

    rotator.SetTransformation(dirRotation, angle);
    rayOrigin = rotator.Transform(rayOrigin);
    rayDirection = rotator.Transform(rayDirection);

    double a = rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y;
    double b = 2.0 * (rayOrigin.x * rayDirection.x + rayOrigin.y * rayDirection.y);
    double c = rayOrigin.x * rayOrigin.x + rayOrigin.y * rayOrigin.y - radius * radius;

    // Solves Quadratic Equation
    double tol = 0.0001;
    double t1 = 0.0;
    double t2 = 0.0;
    bool hit = true;
    double ref = ((rayDirection.z > 0) - (rayDirection.z < 0)) * height;
    if (std::abs(a) < tol)
    {
        if (c > tol)
        {
            hit = false;
        }
        else
        {
            t1 = (-ref - rayOrigin.z) / rayDirection.z;
            t2 = (ref - rayOrigin.z) / rayDirection.z;
        }
    }
    else
    {
        hit = solveq(a, b, c, t1, t2, tol);

        if (hit)
        {
            double z1 = rayOrigin.z + t1 * rayDirection.z;
            double z2 = rayOrigin.z + t2 * rayDirection.z;
            if ((z1 > height && z2 > height) || (z1 < -height && z2 < -height))
            {
                hit = false;
            }
            else if (std::abs(rayDirection.z) > 0.0)
            {
                if (std::abs(z1) > height)
                {
                    t1 += (ref - z1) / rayDirection.z;
                }
                if (std::abs(z2) > height)
                {
                    t2 += (ref - z2) / rayDirection.z;
                }
            }
        }
    }

    Point collisionPoint = Point(0.0, 0.0, 0.0);
    if (t1 >= 0)
    {
        *results = t1;
        collisionPoint = rayOrigin + t1 * rayDirection;
    }
    else if (t2 >= 0)
    {
        *results = t2;
        collisionPoint = rayOrigin + t2 * rayDirection;
    }
    else
    {
        *results = 0.0;
        hit = false;
    }
    if (collision != NULL)
    {
        if (hit)
        {
            this->collisionCount += 1;

            double r = std::sqrt(collisionPoint.x * collisionPoint.x + collisionPoint.y * collisionPoint.y);
            double th = std::atan2(-collisionPoint.x, -collisionPoint.y) + std::acos(-1.0);
            double z = collisionPoint.z + height;

            if (z >= 2.0 * height || z <= 0.0)
            {
                std::cout << "\n";
                std::cout << t1 << " " << t2 << "\n";
                std::cout << rayOrigin.x << " " << rayOrigin.y << " " << rayOrigin.z << "\n";
                std::cout << rayDirection.x << " " << rayDirection.y << " " << rayDirection.z << "\n";
                std::cout << r / radius << " " << th / (2.0 * std::acos(-1.0)) << " " << z / (2.0 * height) << "\n";
                std::cout << "\tCollision in the top/bottom area.\n\n";
            }
            else
            {
                th = th * Lth / (2.0 * std::acos(-1.0));
                z = z * Lz / (2.0 * height);
                collision[((unsigned)z) * Lth + ((unsigned)th)] += 1u;
            }
        }
        else
        {
            // std::cout << "\tCollision missed.\n";
        }
    }
    return hit;
}

void Cylinder::SetupCollision(unsigned Lr, unsigned Lth, unsigned Lz)
{
    this->Lr = Lr;
    this->Lth = Lth;
    this->Lz = Lz;
    this->collision = new long long unsigned[Lth * Lz];
    for (unsigned i = 0u; i < Lth * Lz; i++)
    {
        this->collision[i] = 0u;
    }
}

long long unsigned Cylinder::GetCollision()
{
    return this->collisionCount;
}

long long unsigned Cylinder::GetCollision(unsigned i, unsigned j, unsigned k)
{
    return this->collision[k * Lth + j];
}

void Cylinder::DeleteCollision()
{
    this->Lr = 0u;
    this->Lth = 0u;
    this->Lz = 0u;
    if (this->collision != NULL)
    {
        delete[] this->collision;
    }
}

bool Quad::Intersect(Ray &ray, double *results)
{
    Rotator rotator;
    Point dirRotation = Point(0, 0, 1) & normal;
    Point rayOrigin = ray.origin - center;
    Point rayDirection = ray.direction;
    double angleS = std::asin(std::sqrt(dirRotation | dirRotation) / sqrt(normal | normal));
    double angleC = std::acos(normal.z / std::sqrt(normal | normal));
    double angle = ((angleS > 0) - (angleS < 0)) * angleC;

    rotator.SetTransformation(dirRotation, angle);
    rayOrigin = rotator.Transform(rayOrigin);
    rayDirection = rotator.Transform(rayDirection);

    if (rayDirection.z < 0)
    {
        Point a = rayOrigin + rayOrigin.z * rayDirection;
        if (a.x <= width / 2.0 && a.x >= -width / 2.0 && a.y <= height / 2.0 && a.y >= -height / 2.0)
        {
            unsigned dx = width / Lx;
            unsigned dy = height / Ly;

            this->collisionCount += 1;
            collision[((unsigned)(a.y / dy)) * Lx + (unsigned)(a.x / dx)] += 1u;
            return true;
        }
    }
    return false;
}

void Quad::SetupCollision(unsigned Lx, unsigned Ly)
{
    this->Lx = Lx;
    this->Ly = Ly;
    this->collision = new long long unsigned[Lx * Ly];
    for (unsigned i = 0u; i < Lx * Ly; i++)
    {
        this->collision[i] = 0u;
    }
}

long long unsigned Quad::GetCollision()
{
    return this->collisionCount;
}

long long unsigned Quad::GetCollision(unsigned i, unsigned j)
{
    return this->collision[j * Lx + i];
}

void Quad::DeleteCollision()
{
    this->Lx = 0u;
    this->Ly = 0u;
    if (this->collision != NULL)
    {
        delete[] this->collision;
    }
}