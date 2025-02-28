#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <unordered_map>
#include <fstream>
#include "molecule.h"

// Function declarations
std::vector<std::array<double, 3>> readXYZ(const std::string &filename);
double calculatePotentialEnergy(const std::vector<Molecule> &molecules, double boxSize);
double calculatePotentialEnergyLinkedCells(const std::vector<Molecule> &molecules, double boxSize, int numCells);
int determineOptimalCellCount(double boxSize, size_t numMolecules);

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

    // 1) Read positions
    auto positions = readXYZ(argv[2]);
    std::cout << "Positions read: " << positions.size() << std::endl;

    // 2) Read velocities (optional)
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

    // 3) Create Molecule objects
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

    // 4) Compute kinetic energy
    double E_kin = 0.0;
    for (auto &mol : molecules)
    {
        E_kin += mol.kinetic_energy();
    }
    std::cout << "Computed kinetic energy: " << E_kin << std::endl;

    // 5) Compute potential energy using original method and measure time
    auto start = std::chrono::high_resolution_clock::now();
    double E_pot_orig = calculatePotentialEnergy(molecules, boxSize);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_orig = end - start;

    // Determine optimal cell count based on box size and molecule count
    int cellCount = determineOptimalCellCount(boxSize, molecules.size());

    if (cellCount > 0)
    {
        std::cout << "# Linked cell " << cellCount << " x " << cellCount << " x " << cellCount << " grid set up.\n#\n";

        // 6) Compute potential energy using linked cells and measure time
        start = std::chrono::high_resolution_clock::now();
        double E_pot_cells = calculatePotentialEnergyLinkedCells(molecules, boxSize, cellCount);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed_cells = end - start;

        // 7) Output results and speedup
        std::cout << "E_pot = " << E_pot_cells << ". (Using linked-cell data structure, taking "
                  << elapsed_cells.count() << " ms.)\n";
        std::cout << "E_pot = " << E_pot_orig << ". (Computed without using linked cells, taking "
                  << elapsed_orig.count() << " ms.)\n";
        std::cout << "#\n";

        double speedup = elapsed_orig.count() / elapsed_cells.count();
        std::cout << "Speedup factor from linked cells: " << speedup << std::endl;
    }
    else
    {
        std::cout << "# No linked cell grid used (system too small for optimization).\n#\n";
        std::cout << "E_pot = " << E_pot_orig << ". (Computed without using linked cells, taking "
                  << elapsed_orig.count() << " ms.)\n";
    }

    double E_total = E_kin + E_pot_orig;
    std::cout << "E_kin = " << E_kin << "\n"
              << "E_pot = " << E_pot_orig << "\n"
              << "E_kin + E_pot = " << E_total << "\n";

    return 0;
}

// Function to determine the optimal cell count based on system size and molecule count
int determineOptimalCellCount(double boxSize, size_t numMolecules)
{
    // Minimum cell size is 2.5 (cutoff distance)
    // Maximum sensible cell count for boxSize 20 would be 8 (2.5 per cell)

    // For very small systems or low density, don't use linked cells
    if (numMolecules < 100 || boxSize < 7.5)
    {
        return 0; // No linked cells
    }

    // Calculate a cell count that would give cells slightly larger than cutoff distance
    int cellCount = static_cast<int>(boxSize / 2.5);

    // Make sure we have at least 2 cells per dimension
    cellCount = std::max(2, cellCount);

    // For very large systems, cap at a reasonable number
    cellCount = std::min(cellCount, 20);

    return cellCount;
}

// Original method to calculate potential energy
double calculatePotentialEnergy(const std::vector<Molecule> &molecules, double boxSize)
{
    double E_pot = 0.0;
    std::size_t N = molecules.size();

    for (std::size_t i = 0; i < N; i++)
    {
        for (std::size_t j = i + 1; j < N; j++)
        {
            E_pot += molecules[i].potential_energy(molecules[j], boxSize);
        }
    }

    return E_pot;
}

// Linked cell method to calculate potential energy
double calculatePotentialEnergyLinkedCells(const std::vector<Molecule> &molecules, double boxSize, int numCells)
{
    double E_pot = 0.0;
    double cellSize = boxSize / numCells;

    // Create a 3D grid of cells, each containing a list of molecule indices
    std::vector<std::vector<std::vector<std::vector<size_t>>>> cells(
        numCells, std::vector<std::vector<std::vector<size_t>>>(
                      numCells, std::vector<std::vector<size_t>>(
                                    numCells, std::vector<size_t>())));

    // Sort molecules into cells
    for (size_t i = 0; i < molecules.size(); i++)
    {
        const auto &coords = molecules[i].getCoordinates();

        // Calculate cell indices, handling periodic boundary conditions
        int x = static_cast<int>(std::floor(coords[0] / cellSize)) % numCells;
        int y = static_cast<int>(std::floor(coords[1] / cellSize)) % numCells;
        int z = static_cast<int>(std::floor(coords[2] / cellSize)) % numCells;

        // Handle negative coordinates properly
        if (x < 0)
            x += numCells;
        if (y < 0)
            y += numCells;
        if (z < 0)
            z += numCells;

        cells[x][y][z].push_back(i);
    }

    // Process each cell
    for (int x = 0; x < numCells; x++)
    {
        for (int y = 0; y < numCells; y++)
        {
            for (int z = 0; z < numCells; z++)
            {
                const auto &currentCell = cells[x][y][z];

                // Process molecules within the same cell
                for (size_t i = 0; i < currentCell.size(); i++)
                {
                    for (size_t j = i + 1; j < currentCell.size(); j++)
                    {
                        E_pot += molecules[currentCell[i]].potential_energy(molecules[currentCell[j]], boxSize);
                    }
                }

                // Process interactions with neighboring cells
                // Loop over the 26 neighboring cells
                for (int dx = -1; dx <= 1; dx++)
                {
                    for (int dy = -1; dy <= 1; dy++)
                    {
                        for (int dz = -1; dz <= 1; dz++)
                        {
                            if (dx == 0 && dy == 0 && dz == 0)
                                continue; // Skip the current cell (already processed)

                            // Calculate neighbor cell indices with periodic boundary conditions
                            int nx = (x + dx + numCells) % numCells;
                            int ny = (y + dy + numCells) % numCells;
                            int nz = (z + dz + numCells) % numCells;

                            const auto &neighborCell = cells[nx][ny][nz];

                            // Calculate interactions between molecules in current cell and neighbor cell
                            for (size_t i = 0; i < currentCell.size(); i++)
                            {
                                for (size_t j = 0; j < neighborCell.size(); j++)
                                {
                                    // Avoid double counting by only processing each pair once
                                    if (currentCell[i] < neighborCell[j])
                                    {
                                        E_pot += molecules[currentCell[i]].potential_energy(molecules[neighborCell[j]], boxSize);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return E_pot;
}