#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <fstream>
#include "molecule.h"
#include "molecularsystem.h"

std::vector<std::array<double, 3>> readXYZ(const std::string &filename);

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <box_size> <positions_file> [<velocities_file>]\n";
        return 1;
    }

    double boxSize = std::atof(argv[1]);
    if (boxSize < 5.0)
    {
        std::cerr << "Error: box_size must be at least 5.\n";
        return 1;
    }

    // Read positions
    auto positions = readXYZ(argv[2]);
    std::cout << "Positions read: " << positions.size() << std::endl;

    // Read velocities if provided
    std::vector<std::array<double, 3>> velocities;
    if (argc == 4)
    {
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

    // Initilize molecular system
    MolecularSystem system(boxSize);

    // Add molecules to the system
    for (std::size_t i = 0; i < positions.size(); ++i)
    {
        system.addMolecule(Molecule(
            static_cast<int>(i),
            positions[i][0], positions[i][1], positions[i][2],
            velocities[i][0], velocities[i][1], velocities[i][2]));
    }
    std::cout << "Total molecules created: " << positions.size() << std::endl;

    // Compute kinetic energy
    double E_kin = system.totalKineticEnergy();
    std::cout << "Computed kinetic energy: " << E_kin << std::endl;

    // Compute potential energy using original method and measure time
    auto start = std::chrono::high_resolution_clock::now();
    double E_pot_orig = system.totalPotentialEnergy();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_orig = end - start;

    // Compute potential energy using linked cells and measure time
    start = std::chrono::high_resolution_clock::now();
    double E_pot_cells = system.totalPotentialEnergyLinkedCells();
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_cells = end - start;

    // Output results and speedup
    std::cout << "E_pot = " << E_pot_cells << ". (Using linked-cell data structure, taking "
              << elapsed_cells.count() << " ms.)\n";
    std::cout << "E_pot = " << E_pot_orig << ". (Computed without using linked cells, taking "
              << elapsed_orig.count() << " ms.)\n";
    std::cout << "#\n";

    if (elapsed_cells.count() > 0)
    {
        double speedup = elapsed_orig.count() / elapsed_cells.count();
        std::cout << "Speedup factor from linked cells: " << speedup << std::endl;
    }

    double E_total = E_kin + E_pot_orig;
    std::cout << "E_kin = " << E_kin << "\n"
              << "E_pot = " << E_pot_orig << "\n"
              << "E_kin + E_pot = " << E_total << "\n";

    return 0;
}