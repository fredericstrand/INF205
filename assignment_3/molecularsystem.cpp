#include "molecule.h"
#include "molecularsystem.h"
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

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
    // Using a fixed cell_size of 2.5.
    const double cell_size = 2.5;
    double potential_energy = 0.0;
    int num_cells = static_cast<int>(ceil(box_size / cell_size));
    int num_cells_squared = num_cells * num_cells;
    int num_cells_cubed = num_cells * num_cells * num_cells;

    // Build linked cells using the simplified logic (without fmod)
    vector<vector<size_t>> cells(num_cells_cubed);
    for (size_t i = 0; i < molecules.size(); i++)
    {
        const auto &pos = molecules[i].get_coordinates();
        int cx = static_cast<int>(pos[0] / cell_size) % num_cells;
        int cy = static_cast<int>(pos[1] / cell_size) % num_cells;
        int cz = static_cast<int>(pos[2] / cell_size) % num_cells;
        int idx = cx + cy * num_cells + cz * num_cells_squared;
        cells[idx].push_back(i);
    }

    // Define neighbor offsets (13 neighbors to avoid double counting).
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

    // Loop over all cells.
    for (int cx = 0; cx < num_cells; cx++)
    {
        for (int cy = 0; cy < num_cells; cy++)
        {
            for (int cz = 0; cz < num_cells; cz++)
            {
                int cell_idx = cx + cy * num_cells + cz * num_cells_squared;
                const vector<size_t> &currentCell = cells[cell_idx];

                // Interactions within the same cell.
                for (size_t i = 0; i < currentCell.size(); i++)
                {
                    for (size_t j = i + 1; j < currentCell.size(); j++)
                    {
                        potential_energy += molecules[currentCell[i]].potential_energy(molecules[currentCell[j]], box_size);
                    }
                    // Interactions with neighbor cells.
                    for (int n = 0; n < neighbor_count; n++)
                    {
                        int nx = (cx + neighbor_offsets[n][0] + num_cells) % num_cells;
                        int ny = (cy + neighbor_offsets[n][1] + num_cells) % num_cells;
                        int nz = (cz + neighbor_offsets[n][2] + num_cells) % num_cells;
                        int neighbor_idx = nx + ny * num_cells + nz * num_cells_squared;
                        const vector<size_t> &neighborCell = cells[neighbor_idx];
                        for (size_t j = 0; j < neighborCell.size(); j++)
                        {
                            potential_energy += molecules[currentCell[i]].potential_energy(molecules[neighborCell[j]], box_size);
                        }
                    }
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
