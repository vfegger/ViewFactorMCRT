#include <iostream>
#include <fstream> 
#include <iomanip>
#include <random>
#include <functional>
#include "graphics.hpp"
#include "montecarlo.hpp"

class World
{
public:
    Cylinder base;

    Point emitterCenter;
    Point emitterArea;

    std::fstream fs;

    World(Point baseDirection, Point baseCenter, double baseRadius, double baseHeight, Point emitterCenter, Point emitterArea) : base(baseDirection, baseCenter, baseRadius, baseHeight), emitterCenter(emitterCenter), emitterArea(emitterArea) {}
    void SetupWorld(unsigned Lr, unsigned Lth, unsigned Lz)
    {
        base.SetupCollision(Lr, Lth, Lz);
        fs.open("points.dat", std::fstream::in | std::fstream::out | std::fstream::trunc);
    }
    void DeleteWorld()
    {
        base.DeleteCollision();
        fs.close();
    }
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
            dz = -std::abs(distributionNormal(generator));
            temp = dx * dx + dy * dy + dz * dz;
        } while (temp == 0.0);
        Point direction(dx, dy, dz);
        direction *= 1.0 / sqrt(temp);

        Ray ray(origin, direction);

        Mesh *worldMesh = &base;

        double result = 0.0;
        bool hit = ray.Intersect(worldMesh, 1, result);

        Point col = origin + result * direction;
        fs << direction.x << " " << direction.y << " " << direction.z << " " << hit << " " << col.x << " " << col.y << " " << col.z << "\n";

        // std::cout << "Value t: " << result << " Hit: " << hit << "\n";
        return hit;
    }
};

int main()
{
    std::cout << "MCRT View Factor Estimation\n";

    Point cylinderDirection = Point(1, 0, 0);
    Point cylinderCenter = Point(0, 0, 0);
    double cylinderRadius = 0.0845;
    double cylinderHeight = 0.405;
    Point emitterCenter = Point(0, 0, 0.2585);
    Point emitterArea = Point(0.3, 0.15, 0);
    emitterArea = Point(0.0, 0.0, 0.0);

    unsigned Lr = 1u;
    unsigned Lth = 36u;
    unsigned Lz = 16u;

    World world(cylinderDirection, cylinderCenter, cylinderRadius, cylinderHeight, emitterCenter, emitterArea);
    world.SetupWorld(Lr,Lth,Lz);

    auto fp = std::bind(&World::View, std::ref(world), std::placeholders::_1);

    long long unsigned simulationNumber = 1000000llu;
    long long unsigned seed = 1234u;
    std::cout << "Monte Carlo Method with " << simulationNumber << " elements and seed " << seed << "\n";
    MonteCarlo monteCarlo(seed);
    double result = monteCarlo.Simulate(fp, simulationNumber);
    std::cout << "Result: " << result << "\n";
    unsigned totalCollision = world.base.GetCollision();
    std::cout << "Total Collisions: " << totalCollision << "\n";
    std::cout << "Intenal Result: " << ((double)totalCollision) / simulationNumber << "\n\n";
    std::cout << "Value per cell:\n";
    std::cout << std::setprecision(1);
    unsigned *collisions = new unsigned[Lth * Lz];
    for (unsigned j = 0u; j < Lth; j++)
    {
        for (unsigned k = 0u; k < Lz; k++)
        {
            std::cout << std::scientific << ((double)world.base.GetCollision(0u, j, k)) / totalCollision << " ";
        }
        std::cout << "\n";
    }

    world.DeleteWorld();

    return 0;
}