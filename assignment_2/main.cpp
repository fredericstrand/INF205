#include <iostream>
#include <vector>
#include <string>
#include "molecule.h"

// Function declared in readerXYZ.cpp
std::vector<std::vector<float>> readXYZ(const std::string &filename);

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " positions.xyz velocities.xyz\n";
        return 1;
    }

    // Reading the positions and velocities
    std::vector<std::vector<float>> posData = readXYZ(argv[1]);
    std::vector<std::vector<float>> velData = readXYZ(argv[2]);

    // Initialize the molecules
    std::vector<Molecule> molecules;
    molecules.reserve(posData.size());

    // Create the molecules
    for (size_t i = 0; i < posData.size(); ++i)
    {
        float x = posData[i][0];
        float y = posData[i][1];
        float z = posData[i][2];
        float vx = 0.f, vy = 0.f, vz = 0.f;

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
    float kineticEnergy = 0.0f;
    float potentialEnergy = 0.0f;

    for (const auto &molecule : molecules)
    {
        // Kinetic
        kineticEnergy += molecule.kinetic_energy();

        // Potential (pairwise sums)
        for (size_t i = 0; i < molecules.size(); ++i)
        {
            for (size_t j = i + 1; j < molecules.size(); ++j)
            {
                potentialEnergy += molecules[i].potential_energy(molecules[j]);
            }
        }
    }

    float totalEnergy = kineticEnergy + potentialEnergy;

    std::cout << "Kinetic energy:   " << kineticEnergy << std::endl;
    std::cout << "Potential energy: " << potentialEnergy << std::endl;
    std::cout << "Total energy:     " << totalEnergy << std::endl;

    return 0;
}
