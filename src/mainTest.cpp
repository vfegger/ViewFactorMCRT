#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <functional>
#include "graphics.hpp"
#include "montecarlo.hpp"
#include "Parser.hpp"

void PrintPoint(Point point, std::string name = "")
{
    std::cout << name << " X = " << point.x << "; Y = " << point.y << "; Z = " << point.z << "\n";
}

class World
{
public:
    Quad quad;

    Point emitterCenter;
    Point emitterArea;
    Point emitterNormal;

    std::fstream fs;

    bool makePoints = true;

    World(Point quadNormal, Point quadCenter, double height, double width, Point emitterCenter, Point emitterArea, Point emitterNormal) : quad(quadCenter, quadNormal, height, width), emitterCenter(emitterCenter), emitterArea(emitterArea), emitterNormal(emitterNormal) {}
    void SetupWorld(unsigned Lx, unsigned Ly)
    {
        quad.SetupCollision(Lx, Ly);
        if (makePoints)
        {
            fs.open("data/points.dat", std::fstream::in | std::fstream::out | std::fstream::trunc);
        }
    }
    void DeleteWorld()
    {
        quad.DeleteCollision();
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

        std::uniform_real_distribution<double> distributionNormal(0, 1);
        // Generate two random numbers
        double u1 = distributionNormal(generator);
        double u2 = distributionNormal(generator);

        // Use u1 for cos(theta), theta is angle with the vertical (normal direction)
        double cos_theta = sqrt(1 - u1);
        double sin_theta = sqrt(u1); // sin(theta) = sqrt(1 - cos^2(theta))

        // Use u2 for phi, phi is azimuthal angle, uniformly distributed around the circle
        double phi = 2 * acos(-1.0) * u2;

        // Convert to Cartesian coordinates
        double dx = sin_theta * cos(phi);
        double dy = sin_theta * sin(phi);
        double dz = cos_theta;

        Point direction(dx, dy, dz);

        if (emitterNormal.z < 0.0)
        {
            direction.z = -direction.z;
        }
        else
        {
            Point axis = emitterNormal & Point(0, 0, 1);
            double angle = acos(Point(0, 0, 1) | emitterNormal);

            Rotator rotator;
            rotator.SetTransformation(axis, angle);
            direction = rotator.Transform(direction);
        }

        Ray ray(origin, direction);

        Mesh *worldMesh = &quad;

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

void Problem(unsigned Lx, unsigned Ly, double *collisions)
{
    Point quadNormal = Point(0, 0, 1);
    Point quadCenter = Point(0, 0, 0);
    double quadHeight = 1.0;
    double quadWidth = 1.0;
    Point emitterCenter = Point(0, 0, 1);
    Point emitterArea = Point(0.5, 0.5, 0);
    Point emitterNormal = Point(0, 0, -1);

    World world(quadNormal, quadCenter, quadHeight, quadWidth, emitterCenter, emitterArea, emitterNormal);
    world.SetupWorld(Lx, Ly);

    auto fp = std::bind(&World::View, std::ref(world), std::placeholders::_1);

    long long unsigned simulationNumber = 1llu << 24llu;
    long long unsigned seed = 1234u;
    std::cout << "Monte Carlo Method with " << simulationNumber << " elements and seed " << seed << "\n";
    MonteCarlo monteCarlo(seed);
    double result = monteCarlo.Simulate(fp, simulationNumber);
    std::cout << "Result by Monte Carlo method: " << result << "\n";
    unsigned totalCollision = world.quad.GetCollision();
    std::cout << "Total Collisions: " << totalCollision << "\n";
    std::cout << "Total View Factor: " << ((double)totalCollision) / simulationNumber << "\n\n";
    for (unsigned j = 0u; j < Ly; j++)
    {
        for (unsigned i = 0u; i < Lx; i++)
        {
            collisions[j * Ly + i] = ((double)world.quad.GetCollision(i, j)) / ((double)totalCollision);
        }
    }
    world.DeleteWorld();
}

int main()
{
    std::cout << "MCRT View Factor Estimation\n";

    unsigned Lx = 10u;
    unsigned Ly = 10u;
    double *collisions = new double[Lx * Ly];

    Problem(Lx, Ly, collisions);

    std::string path_dir = std::filesystem::current_path().string();
    std::string path_binary_out = path_dir + "/data/binary/";
    std::string path_text_out = path_dir + "/data/text/";
    std::string name_viewFactor = "ViewFactor";
    name_viewFactor += "X" + std::to_string(Lx) + "Y" + std::to_string(Ly);
    std::string extension_text = ".dat";
    std::string extension_binary = ".bin";

    Parser *parser = new Parser(2u);
    unsigned index = parser->OpenFileOut(path_binary_out, name_viewFactor, extension_binary, std::ios::binary);
    std::streampos position;
    Parser::ExportConfigurationBinary(parser->GetStreamOut(index), "View Factor", Lx * Ly, ParserType::Double, position);

    Parser::ExportValuesBinary(parser->GetStreamOut(index), Lx * Ly, ParserType::Double, collisions, position, 0u);

    Parser::ConvertToText(path_binary_out, path_text_out, extension_text);

    delete parser;
    delete[] collisions;

    return 0;
}