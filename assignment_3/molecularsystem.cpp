#include "molecularsystem.h"
#include "molecule.h"
#include <cmath>
#include <omp.h>
#include <vector>

MolecularSystem::MolecularSystem(double a) : box_size(a) {}

void MolecularSystem::add_molecule(const Molecule &mol)
{
    molecules.push_back(mol);
}

const std::vector<Molecule> &MolecularSystem::get_molecules() const
{
    return molecules;
}

double MolecularSystem::total_kinetic_energy() const
{
    double kinetic_energy = 0.0;
    for (const auto &mol : molecules)
    {
        kinetic_energy += mol.kinetic_energy();
    }
    return kinetic_energy;
}

double MolecularSystem::total_potential_energy() const
{
    double potential_energy = 0.0;
    const size_t n = molecules.size();

#pragma omp parallel for reduction(+ : potential_energy)
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = i + 1; j < n; j++)
        {
            potential_energy += molecules[i].potential_energy(molecules[j], box_size);
        }
    }
    return potential_energy;
}

double MolecularSystem::total_potential_energy_LinkedCells() const
{
    const double cell_size = 2.5;
    double potential_energy = 0.0;
    int num_cells = static_cast<int>(ceil(box_size / cell_size));
    int num_cells_squared = num_cells * num_cells;
    int num_cells_cubed = num_cells * num_cells * num_cells;

    // Build linked cells
    std::vector<std::vector<size_t>> cells(num_cells_cubed);
    for (size_t i = 0; i < molecules.size(); i++)
    {
        const auto &pos = molecules[i].get_coordinates();
        int cx = static_cast<int>(pos[0] / cell_size) % num_cells;
        int cy = static_cast<int>(pos[1] / cell_size) % num_cells;
        int cz = static_cast<int>(pos[2] / cell_size) % num_cells;
        int idx = cx + cy * num_cells + cz * num_cells_squared;
        cells[idx].push_back(i);
    }

    // Precompute neighbor indices to avoid recomputing in inner loops.
    const int neighbor_count = 13;
    const int neighbor_offsets[neighbor_count][3] = {
        {0, 0, 1},
        {0, 1, -1},
        {0, 1, 0},
        {0, 1, 1},
        {1, -1, -1},
        {1, -1, 0},
        {1, -1, 1},
        {1, 0, -1},
        {1, 0, 0},
        {1, 0, 1},
        {1, 1, -1},
        {1, 1, 0},
        {1, 1, 1}};

    std::vector<std::vector<int>> neighbor_indices(num_cells_cubed);
    for (int cx = 0; cx < num_cells; cx++)
    {
        for (int cy = 0; cy < num_cells; cy++)
        {
            for (int cz = 0; cz < num_cells; cz++)
            {
                int cell_idx = cx + cy * num_cells + cz * num_cells_squared;
                std::vector<int> neigh;
                neigh.reserve(neighbor_count); // Reserve fixed space for neighbor indices
                for (int n = 0; n < neighbor_count; n++)
                {
                    int nx = (cx + neighbor_offsets[n][0] + num_cells) % num_cells;
                    int ny = (cy + neighbor_offsets[n][1] + num_cells) % num_cells;
                    int nz = (cz + neighbor_offsets[n][2] + num_cells) % num_cells;
                    int neighbor_idx = nx + ny * num_cells + nz * num_cells_squared;
                    neigh.push_back(neighbor_idx);
                }
                neighbor_indices[cell_idx] = neigh;
            }
        }
    }

#pragma omp parallel for reduction(+ : potential_energy) schedule(dynamic)
    for (int cell_idx = 0; cell_idx < num_cells_cubed; cell_idx++)
    {
        const std::vector<size_t> &currentCell = cells[cell_idx];
        for (size_t i = 0; i < currentCell.size(); i++)
        {
            // Interactions within the same cell.
            for (size_t j = i + 1; j < currentCell.size(); j++)
            {
                potential_energy += molecules[currentCell[i]].potential_energy(molecules[currentCell[j]], box_size);
            }
            // Interactions with neighbor cells.
            for (int neighbor : neighbor_indices[cell_idx])
            {
                const std::vector<size_t> &neighborCell = cells[neighbor];
                for (size_t j = 0; j < neighborCell.size(); j++)
                {
                    potential_energy += molecules[currentCell[i]].potential_energy(molecules[neighborCell[j]], box_size);
                }
            }
        }
    }
    return potential_energy;
}

double MolecularSystem::total_energy() const
{
    return total_kinetic_energy() + total_potential_energy();
}
