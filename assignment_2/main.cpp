#include <iostream>
#include <vector>
#include <string>
#include "molecule.h"

// Function declared in readerXYZ.cpp
std::vector<std::vector<double>> readXYZ(const std::string &filename);

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " positions.xyz velocities.xyz\n";
        return 1;
    }

    // Reading the positions and velocities
    std::vector<std::vector<double>> posData = readXYZ(argv[1]);
    std::vector<std::vector<double>> velData = readXYZ(argv[2]);

    // Initialize the molecules
    std::vector<Molecule> molecules;
    molecules.reserve(posData.size());

    // Create the molecules
    for (size_t i = 0; i < posData.size(); ++i)
    {
        double x = posData[i][0];
        double y = posData[i][1];
        double z = posData[i][2];
        double vx = 0.f, vy = 0.f, vz = 0.f;

        if (i < velData.size())
        {
            vx = velData[i][0];
            vy = velData[i][1];
            vz = velData[i][2];
        }

        Molecule mol(static_cast<int>(i), x, y, z, vx, vy, vz);
        molecules.push_back(mol);
    }

    // Compute the total kinetic and potential energy
    double kineticEnergy = 0.0;
    double potentialEnergy = 0.0;

    size_t i = 0;
    for (const auto &molecule : molecules)
    {
        kineticEnergy += molecule.kinetic_energy();

        for (size_t j = i + 1; j < molecules.size(); ++j)
        {
            potentialEnergy += molecule.potential_energy(molecules[j]);
        }

        ++i;
    }

    double totalEnergy = kineticEnergy + potentialEnergy;

    std::cout << "Kinetic energy:   " << kineticEnergy << std::endl;
    std::cout << "Potential energy: " << potentialEnergy << std::endl;
    std::cout << "Total energy:     " << totalEnergy << std::endl;

    return 0;
}
