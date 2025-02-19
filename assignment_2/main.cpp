#include <iostream>
#include <vector>
#include <string>
#include "molecule.h"
#include "molecularsystem.h"

// Initialize READXYZ function
std::vector<std::array<double, 3>> readXYZ(const std::string &filename);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "You must at least have a positions file " << argv[0] << "\n";
        return 1;
    }

    std::vector<std::array<double, 3>> posData;
    std::vector<std::array<double, 3>> velData;

    if (argc == 2)
    {
        posData = readXYZ(argv[1]);
        velData.resize(posData.size(), {0.0, 0.0, 0.0});
    }
    else
    {
        posData = readXYZ(argv[1]);
        velData = readXYZ(argv[2]);
    }

    // Initialize MolecularSystem object
    MolecularSystem system;

    // Loop over all positions and velocities, and create Molecule objects
    for (size_t i = 0; i < posData.size(); ++i)
    {
        double x = posData[i][0];
        double y = posData[i][1];
        double z = posData[i][2];
        double vx = 0.0, vy = 0.0, vz = 0.0;

        // Add velocities if available
        if (i < velData.size())
        {
            vx = velData[i][0];
            vy = velData[i][1];
            vz = velData[i][2];
        }

        // Create Molecule object
        Molecule mol(static_cast<int>(i), x, y, z, vx, vy, vz);

        // Add molecule object to MolecularSystem object
        system.addMolecule(mol);
    }

    double kineticEnergy = system.totalKinetic();
    double potentialEnergy = system.totalPotential();
    double totalEnergy = kineticEnergy + potentialEnergy;

    std::cout << "Kinetic energy:   " << kineticEnergy << std::endl;
    std::cout << "Potential energy: " << potentialEnergy << std::endl;
    std::cout << "Total energy:     " << totalEnergy << std::endl;

    return 0;
}
