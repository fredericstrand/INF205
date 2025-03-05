#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <array>
#include <string>

std::vector<std::array<double, 3>> readXYZ(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Could not open file: " << filename << std::endl;
        return {};
    }

    std::vector<std::array<double, 3>> data;
    std::size_t numAtoms = 0;

    // Read the first line to get the number of atoms
    std::string line;
    if (std::getline(file, line))
    {
        std::istringstream(line) >> numAtoms;
    }

    // Skip the second line
    std::getline(file, line);

    // Read atom data
    std::string atomType;
    double x, y, z;
    while (std::getline(file, line) && data.size() < numAtoms)
    {
        std::istringstream iss(line);
        if (iss >> atomType >> x >> y >> z)
        {
            data.push_back({x, y, z});
        }
    }

    if (data.size() != numAtoms)
    {
        std::cerr << "Warning: Expected " << numAtoms
                  << " atoms, but read " << data.size() << std::endl;
    }

    return data;
}
