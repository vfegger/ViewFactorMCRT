#include "graphics.hpp"
#include "montecarlo.hpp"
#include <iostream>
#include <random>
#include <functional>

class World
{
private:
    Cylinder base;

    Point emitterCenter;
    Point emitterArea;

public:
    World(Point baseDirection, Point baseCenter, double baseRadius, double baseHeight, Point emitterCenter, Point emitterArea) : base(baseDirection, baseCenter, baseRadius, baseHeight), emitterCenter(emitterCenter), emitterArea(emitterArea) {}
    bool View(std::default_random_engine &generator)
    {
        std::uniform_real_distribution<double> distributionX(-emitterArea.x, emitterArea.x);
        std::uniform_real_distribution<double> distributionY(-emitterArea.y, emitterArea.y);
        double x = emitterCenter.x + distributionX(generator);
        double y = emitterCenter.y + distributionY(generator);
        double z = emitterCenter.z;
        Point origin(x, y, z);

        double dx, dy, dz;
        std::normal_distribution<double> distributionNormal(0, 1);
        double temp = 0.0;
        do
        {
            dx = distributionNormal(generator);
            dy = distributionNormal(generator);
            dz = distributionNormal(generator);
            temp = dx * dx + dy * dy + dz * dz;
        } while (temp == 0.0);
        Point direction(dx, dy, dz);
        direction *= 1.0/sqrt(temp);

        Ray ray(origin, direction);

        ray.direction.x = 0.0;
        ray.direction.y = 0.0;
        ray.direction.z = -1.0;

        Mesh *worldMesh = &base;

        double result = 0.0;
        bool hit = ray.Intersect(worldMesh, 1, result);

        std::cout << "\tOrigin: " << origin.x << " " << origin.y << " " << origin.z << "\n";
        std::cout << "\tDirection: " << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << "\n";
        std::cout << "\tHit: " << hit << "\n\n";
        return hit;
    }
};

int main()
{
    std::cout << "MCRT View Factor Estimation\n";

    Point cylinderDirection = Point(1, 0, 0);
    Point cylinderCenter = Point(0, 0, 0);
    double cylinderRadius = 1.0;
    double cylinderHeight = 1.0;
    Point emitterCenter = Point(0, 0, 10);
    Point emitterArea = Point(1, 1, 0);
    World world(cylinderDirection, cylinderCenter, cylinderRadius, cylinderHeight,emitterCenter,emitterArea);

    auto fp = std::bind(&World::View,world,std::placeholders::_1);
    
    long long unsigned simulationNumber = 1llu;
    long long unsigned seed = 1234u;
    std::cout << "Monte Carlo Method with " << simulationNumber << " elements and seed " << seed << "\n";
    MonteCarlo monteCarlo(seed);
    double result = monteCarlo.Simulate(fp,simulationNumber);


    std::cout << "Result: " << result << "\n";
    return 0;
}