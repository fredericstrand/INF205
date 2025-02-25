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

    int box_size = argv[1];
    std::vector<std::array<double, 3>> posData;
    std::vector<std::array<double, 3>> velData;

    if (argc == 3)
    {
        posData = readXYZ(argv[1]);
        velData.resize(posData.size(), {0.0, 0.0, 0.0});
    }
    else
    {
        posData = readXYZ(argv[2]);
        velData = readXYZ(argv[3]);
    }

    // Initialize MolecularSystem object
    MolecularSystem system;
    for (size_t i = 0; i < posData.size(); ++i)
    {
        const auto &pos = posData[i];
        const auto &vel = velData[i];

        // Directly add a Molecule using an initializer list.
        system.addMolecule(Molecule(static_cast<int>(i), pos[0], pos[1], pos[2],
                                    vel[0], vel[1], vel[2]));
    }

    double kineticEnergy = system.totalKinetic();
    double potentialEnergy = system.totalPotential();
    double totalEnergy = kineticEnergy + potentialEnergy;

    std::cout << "Kinetic energy:   " << kineticEnergy << std::endl;
    std::cout << "Potential energy: " << potentialEnergy << std::endl;
    std::cout << "Total energy:     " << totalEnergy << std::endl;

    return 0;
}
