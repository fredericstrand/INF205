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

// Declarations for file reading
std::vector<std::array<double, 3>> readXYZPositions(double a, const std::string &filename);
std::vector<std::array<double, 3>> readXYZVelocities(const std::string &filename);

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <box_size> <positions_file> [<velocities_file>]\n";
        return 1;
    }

    double box_size = std::atof(argv[1]);
    if (box_size < 5.0)
    {
        std::cerr << "Error: box_size must be at least 5.\n";
        return 1;
    }

    // Read positions
    auto positions = readXYZPositions(box_size, argv[2]);
    auto positions = readXYZ(argv[2]);
    std::cout << "Positions read: " << positions.size() << std::endl;

    // Read velocities if provided
    std::vector<std::array<double, 3>> velocities;
    if (argc == 4)
    {
        velocities = readXYZVelocities(argv[3]);
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

    // Create molecular system
    MolecularSystem system(box_size);
    // Initilize molecular system
    MolecularSystem system(boxSize);

    // Add molecules to the system
    for (std::size_t i = 0; i < positions.size(); ++i)
    {
        system.add_molecule(Molecule(
        system.addMolecule(Molecule(
            static_cast<int>(i),
            positions[i][0], positions[i][1], positions[i][2],
            velocities[i][0], velocities[i][1], velocities[i][2]));
    }
    std::cout << "Total molecules created: " << positions.size() << std::endl;

    // Compute kinetic energy
    double E_kin = system.total_kinetic_energy();

    auto start = std::chrono::steady_clock::now();
    double E_pot_orig = system.total_potential_energy();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed_orig = end - start;

    start = std::chrono::steady_clock::now();
    double E_pot_cells = system.total_potential_energy_LinkedCells();
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed_cells = end - start;

    // Output results and speedup
    std::cout << "E_pot = " << E_pot_cells << ". (Linked cells, " << elapsed_cells.count() << " ms.)\n";
    std::cout << "E_pot = " << E_pot_orig << ". (Original, " << elapsed_orig.count() << " ms.)\n";

    double E_kin = system.totalKineticEnergy();
    std::cout << "Computed kinetic energy: " << E_kin << std::endl;

    // Compute potential energy using original method
    auto start = std::chrono::high_resolution_clock::now();
    double E_pot_orig = system.totalPotentialEnergy();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_orig = end - start;

    // Compute potential energy using linked cells
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