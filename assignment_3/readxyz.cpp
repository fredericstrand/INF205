#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <cmath>

std::vector<std::array<double, 3>> readXYZPositions(double a, const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Could not open file: " << filename << "\n";
        return {};
    }
    std::vector<std::array<double, 3>> data;
    std::size_t numAtoms = 0;
    std::string line;
    if (std::getline(file, line))
    {
        std::istringstream iss(line);
        iss >> numAtoms;
    }
    std::getline(file, line);
    std::string atom;
    double x, y, z;
    while (std::getline(file, line) && data.size() < numAtoms)
    {
        std::istringstream iss(line);
        if (iss >> atom >> x >> y >> z)
        {
            x = fmod(x, a);
            if (x < 0)
                x += a;
            y = fmod(y, a);
            if (y < 0)
                y += a;
            z = fmod(z, a);
            if (z < 0)
                z += a;
            data.push_back({x, y, z});
        }
    }
    if (data.size() != numAtoms)
        std::cerr << "Warning: Expected " << numAtoms << " atoms, but read " << data.size() << "\n";
    return data;
}

std::vector<std::array<double, 3>> readXYZVelocities(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Could not open file: " << filename << "\n";
        return {};
    }
    std::vector<std::array<double, 3>> data;
    std::size_t numAtoms = 0;
    std::string line;
    if (std::getline(file, line))
    {
        std::istringstream iss(line);
        iss >> numAtoms;
    }
    std::getline(file, line);
    std::string atom;
    double vx, vy, vz;
    while (std::getline(file, line) && data.size() < numAtoms)
    {
        std::istringstream iss(line);
        if (iss >> atom >> vx >> vy >> vz)
        {
            data.push_back({vx, vy, vz});
        }
    }
    if (data.size() != numAtoms)
        std::cerr << "Warning: Expected " << numAtoms << " atoms, but read " << data.size() << "\n";
    return data;
}
