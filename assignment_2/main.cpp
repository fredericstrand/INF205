#include <iostream>
#include <vector>
#include <string>
#include "molecule.h"
#include "molecularsystem.h"

// Initialize READXYZ function
std::vector<std::vector<double>> readXYZ(const std::string &filename);

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " positions.xyz velocities.xyz\n";
        return 1;
    }

    // Read data from files
    std::vector<std::vector<double>> posData = readXYZ(argv[1]);
    std::vector<std::vector<double>> velData = readXYZ(argv[2]);

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
