#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include "molecule.h"

std::vector<std::array<double, 3>> readXYZ(const std::string &filename);

int main(int argc, char *argv[])
{
    std::cout << "Program started..." << std::endl;

    if (argc < 3 || argc > 4)
    {
        std::cerr << "Usage: " << argv[0] << " <box_size> <positions_file> [<velocities_file>]\n";
        return 1;
    }

    double boxSize = std::atof(argv[1]);
    if (boxSize < 5.0)
    {
        std::cerr << "Error: box_size must be at least 5.\n";
        return 1;
    }

    std::cout << "Reading positions from: " << argv[2] << std::endl;
    auto positions = readXYZ(argv[2]);
    std::cout << "Positions read: " << positions.size() << std::endl;

    std::vector<std::array<double, 3>> velocities;

    if (argc == 4)
    {
        std::cout << "Reading velocities from: " << argv[3] << std::endl;
        velocities = readXYZ(argv[3]);
        std::cout << "Velocities read: " << velocities.size() << std::endl;

        if (positions.size() != velocities.size())
        {
            std::cerr << "Error: positions and velocities must have the same size.\n";
            return 1;
        }
    }
    else
    {
        std::cout << "No velocity file provided. Setting all velocities to (0,0,0)." << std::endl;
        velocities.resize(positions.size(), {0.0, 0.0, 0.0});
    }

    std::cout << "Building molecule list..." << std::endl;
    std::vector<Molecule> molecules;
    molecules.reserve(positions.size());
    for (std::size_t i = 0; i < positions.size(); ++i)
    {
        molecules.emplace_back(
            static_cast<int>(i),
            positions[i][0], positions[i][1], positions[i][2],
            velocities[i][0], velocities[i][1], velocities[i][2]);
    }

    std::cout << "Total molecules created: " << molecules.size() << std::endl;

    double E_kin = 0.0;
    for (auto &mol : molecules)
    {
        E_kin += mol.kinetic_energy();
    }
    std::cout << "Computed kinetic energy: " << E_kin << std::endl;

    double E_pot = 0.0;
    std::size_t N = molecules.size();
    for (std::size_t i = 0; i < N; i++)
    {
        for (std::size_t j = i + 1; j < N; j++)
        {
            E_pot += molecules[i].potential_energy(molecules[j], boxSize);
        }
    }
    std::cout << "Computed potential energy: " << E_pot << std::endl;

    double E_total = E_kin + E_pot;

    std::cout << "E_kin = " << E_kin << "\n";
    std::cout << "E_pot = " << E_pot << "\n";
    std::cout << "E_kin + E_pot = " << E_total << "\n";

    return 0;
}
