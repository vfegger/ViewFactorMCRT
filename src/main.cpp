#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <functional>
#include "graphics.hpp"
#include "montecarlo.hpp"
#include "Parser.hpp"

class World
{
public:
    Cylinder base;

    Point emitterCenter;
    Point emitterArea;
    Point emitterNormal;

    std::fstream fs;

    bool makePoints = true;

    World(Point baseDirection, Point baseCenter, double baseRadius, double baseHeight, Point emitterCenter, Point emitterArea, Point emitterNormal) : base(baseDirection, baseCenter, baseRadius, baseHeight), emitterCenter(emitterCenter), emitterArea(emitterArea), emitterNormal(emitterNormal) {}
    void SetupWorld(unsigned Lr, unsigned Lth, unsigned Lz)
    {
        base.SetupCollision(Lr, Lth, Lz);
        if (makePoints)
        {
            fs.open("data/points.dat", std::fstream::in | std::fstream::out | std::fstream::trunc);
        }
    }
    void DeleteWorld()
    {
        base.DeleteCollision();
        if (makePoints)
        {
            fs.close();
        }
    }
    bool View(std::default_random_engine &generator)
    {
        std::uniform_real_distribution<double> distributionX(-emitterArea.x, emitterArea.x);
        std::uniform_real_distribution<double> distributionY(-emitterArea.y, emitterArea.y);
        std::uniform_real_distribution<double> distributionZ(-emitterArea.z, emitterArea.z);
        double x = emitterCenter.x + distributionX(generator);
        double y = emitterCenter.y + distributionY(generator);
        double z = emitterCenter.z + distributionZ(generator);

        Point origin(x, y, z);

        double dx, dy, dz;
        std::normal_distribution<double> distributionNormal(0, 1);
        double temp = 0.0;
        Point direction;
        do
        {
            dx = distributionNormal(generator);
            dy = distributionNormal(generator);
            dz = distributionNormal(generator);
            direction = Point(dx, dy, dz);
            temp = (direction | direction);
        } while ((direction | emitterNormal) < 0.0 || temp == 0.0);
        direction *= 1.0 / sqrt(temp);

        Ray ray(origin, direction);

        Mesh *worldMesh = &base;

        double result = 0.0;
        bool hit = ray.Intersect(worldMesh, 1, result);

        Point col = origin + result * direction;

        if (makePoints)
        {
            fs << origin.x << "\t" << origin.y << "\t" << origin.z << "\t";
            fs << direction.x << "\t" << direction.y << "\t" << direction.z << "\t";
            fs << col.x << "\t" << col.y << "\t" << col.z << "\t";
            fs << hit << "\t" << result << "\n";
        }

        return hit;
    }
};

void Problem(unsigned Lr, unsigned Lth, unsigned Lz, double *collisions)
{
    Point cylinderDirection = Point(0, 0, 1);
    Point cylinderCenter = Point(0, 0, 0);
    double cylinderRadius = 0.169 / 2.0;
    double cylinderHeight = 0.81 / 2.0;
    double cylinderDistanceEmitter = 0.174;
    Point emitterCenter = Point(cylinderRadius + cylinderDistanceEmitter, 0, 0);
    Point emitterArea = Point(0, 0.015, 0.03);
    Point emitterNormal = Point(-1, 0, 0);

    World world(cylinderDirection, cylinderCenter, cylinderRadius, cylinderHeight, emitterCenter, emitterArea, emitterNormal);
    world.SetupWorld(Lr, Lth, Lz);

    auto fp = std::bind(&World::View, std::ref(world), std::placeholders::_1);

    long long unsigned simulationNumber = 1llu << 20llu;
    long long unsigned seed = 1234u;
    std::cout << "Monte Carlo Method with " << simulationNumber << " elements and seed " << seed << "\n";
    MonteCarlo monteCarlo(seed);
    double result = monteCarlo.Simulate(fp, simulationNumber);
    std::cout << "Result by Monte Carlo method: " << result << "\n";
    unsigned totalCollision = world.base.GetCollision();
    std::cout << "Total Collisions: " << totalCollision << "\n";
    std::cout << "Total View Factor: " << ((double)totalCollision) / simulationNumber << "\n\n";
    for (unsigned k = 0u; k < Lz; k++)
    {
        for (unsigned j = 0u; j < Lth; j++)
        {
            collisions[k * Lth + j] = ((double)world.base.GetCollision(0u, j, k)) / ((double)totalCollision);
        }
    }
    world.DeleteWorld();
}

int main()
{
    std::cout << "MCRT View Factor Estimation\n";

    unsigned Lr = 1u;
    unsigned Lth = 36u;
    unsigned Lz = 16u;
    double *collisions = new double[Lth * Lz];

    Problem(Lr, Lth, Lz, collisions);

    std::string path_dir = std::filesystem::current_path().string();
    std::string path_binary_out = path_dir + "/data/binary/";
    std::string path_text_out = path_dir + "/data/text/";
    std::string name_viewFactor = "viewFactor";
    std::string extension_text = ".dat";
    std::string extension_binary = ".bin";

    Parser *parser = new Parser(2u);
    unsigned index = parser->OpenFileOut(path_binary_out, name_viewFactor, extension_binary, std::ios::binary);
    std::streampos position;
    Parser::ExportConfigurationBinary(parser->GetStreamOut(index), "View Factor", Lth * Lz, ParserType::Double, position);

    Parser::ExportValuesBinary(parser->GetStreamOut(index), Lth * Lz, ParserType::Double, collisions, position, 0u);

    Parser::ConvertToText(path_binary_out, path_text_out, extension_text);

    delete parser;
    delete[] collisions;

    return 0;
}