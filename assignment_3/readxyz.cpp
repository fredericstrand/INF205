#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <array>

std::vector<std::array<double, 3>> readXYZ(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::vector<std::array<double, 3>> coordinates;
    std::string line, atomType;
    std::size_t numAtoms;

    // Read number of atoms
    std::getline(file, line);
    std::istringstream(line) >> numAtoms;

    // Skip comment line
    std::getline(file, line);

    // Read atom coordinates
    double x, y, z;
    while (std::getline(file, line) && coordinates.size() < numAtoms)
    {
        std::istringstream iss(line);
        if (iss >> atomType >> x >> y >> z)
        {
            coordinates.push_back({x, y, z});
        }
    }

    if (coordinates.size() != numAtoms)
    {
        std::cerr << "Warning: Expected " << numAtoms << " atoms, but read "
                  << coordinates.size() << std::endl;
    }

    return coordinates;
}