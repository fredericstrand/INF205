#include "molecularsystem.h"
#include <cmath>
#include <algorithm>
#include "molecule.h"

MolecularSystem::MolecularSystem(double boxSize) : m_boxSize(boxSize)
{
}

void MolecularSystem::addMolecule(const Molecule &molecule)
{
    m_molecules.push_back(molecule);
}

double MolecularSystem::totalKineticEnergy() const
{
    double E_kin = 0.0;
    for (const auto &molecule : m_molecules)
    {
        E_kin += molecule.kinetic_energy();
    }
    return E_kin;
}

double MolecularSystem::totalPotentialEnergy() const
{
    double E_pot = 0.0;
    const size_t N = m_molecules.size();

    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = i + 1; j < N; j++)
        {
            E_pot += m_molecules[i].potential_energy(m_molecules[j], m_boxSize);
        }
    }

    return E_pot;
}

int MolecularSystem::determineOptimalCellCount() const
{
    // Minimum cell size is 2.5 (cutoff distance)
    // For very small systems or low density, don't use linked cells
    if (m_molecules.size() < 100 || m_boxSize < 7.5)
    {
        return 0; // No linked cells
    }

    // Calculate cell count to give cells slightly larger than cutoff distance
    int cellCount = static_cast<int>(m_boxSize / 2.5);

    // Make sure we have at least 2 cells per dimension
    cellCount = std::max(2, cellCount);

    // For very large systems, cap at a reasonable number
    cellCount = std::min(cellCount, 20);

    return cellCount;
}

double MolecularSystem::totalPotentialEnergyLinkedCells() const
{
    int numCells = determineOptimalCellCount();

    // If system is too small, revert to original method
    if (numCells <= 0)
    {
        return totalPotentialEnergy();
    }

    double E_pot = 0.0;
    double cellSize = m_boxSize / numCells;

    // Create a 3D grid of cells, each containing a list of molecule indices
    std::vector<std::vector<std::vector<std::vector<size_t>>>> cells(
        numCells, std::vector<std::vector<std::vector<size_t>>>(
                      numCells, std::vector<std::vector<size_t>>(
                                    numCells, std::vector<size_t>())));

    // Sort molecules into cells
    for (size_t i = 0; i < m_molecules.size(); i++)
    {
        const auto &coords = m_molecules[i].getCoordinates();

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
                        E_pot += m_molecules[currentCell[i]].potential_energy(
                            m_molecules[currentCell[j]], m_boxSize);
                    }
                }

                // Process interactions with neighboring cells
                for (int dx = -1; dx <= 1; dx++)
                {
                    for (int dy = -1; dy <= 1; dy++)
                    {
                        for (int dz = -1; dz <= 1; dz++)
                        {
                            if (dx == 0 && dy == 0 && dz == 0)
                                continue; // Skip current cell (already processed)

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
                                        E_pot += m_molecules[currentCell[i]].potential_energy(
                                            m_molecules[neighborCell[j]], m_boxSize);
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

double MolecularSystem::totalEnergy() const
{
    return totalKineticEnergy() + totalPotentialEnergy();
}

const std::vector<Molecule> &MolecularSystem::getMolecules() const
{
    return m_molecules;
}