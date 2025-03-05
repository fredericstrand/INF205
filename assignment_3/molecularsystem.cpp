#include "molecularsystem.h"
#include <cmath>
#include <algorithm>

MolecularSystem::MolecularSystem(double boxSize)
    : m_boxSize(boxSize)
{
}

void MolecularSystem::addMolecule(const Molecule &molecule)
{
    m_molecules.push_back(molecule);
}

double MolecularSystem::totalKineticEnergy() const
{
    double E_kin = 0.0;
    for (const auto &m : m_molecules)
    {
        E_kin += m.kinetic_energy();
    }
    return E_kin;
}

double MolecularSystem::totalPotentialEnergy() const
{
    double E_pot = 0.0;
#pragma omp parallel for reduction(+ : E_pot)
    for (size_t i = 0; i < m_molecules.size(); i++)
    {
        for (size_t j = i + 1; j < m_molecules.size(); j++)
        {
            E_pot += m_molecules[i].potential_energy(m_molecules[j], m_boxSize);
        }
    }
    return E_pot;
}

int MolecularSystem::determineOptimalCellCount() const
{
    if (m_molecules.size() < 100 || m_boxSize < 7.5)
        return 0;

    int cellCount = static_cast<int>(m_boxSize / 2.5);
    cellCount = std::max(cellCount, 2);
    cellCount = std::min(cellCount, 20);
    return cellCount;
}

double MolecularSystem::totalPotentialEnergyLinkedCells() const
{
    int numCells = determineOptimalCellCount();
    if (numCells <= 0)
        return totalPotentialEnergy();

    double cellSize = m_boxSize / numCells;
    std::vector<std::vector<size_t>> cells(numCells * numCells * numCells);

    auto index3D = [&](double x, double y, double z)
    {
        int ix = static_cast<int>(x / cellSize) % numCells;
        int iy = static_cast<int>(y / cellSize) % numCells;
        int iz = static_cast<int>(z / cellSize) % numCells;
        if (ix < 0)
            ix += numCells;
        if (iy < 0)
            iy += numCells;
        if (iz < 0)
            iz += numCells;
        return ix + iy * numCells + iz * numCells * numCells;
    };

    for (size_t i = 0; i < m_molecules.size(); i++)
    {
        auto &c = m_molecules[i].getCoordinates();
        cells[index3D(c[0], c[1], c[2])].push_back(i);
    }

    std::vector<std::array<int, 3>> offsets;
    for (int dx = 0; dx <= 1; dx++)
        for (int dy = 0; dy <= 1; dy++)
            for (int dz = 0; dz <= 1; dz++)
                offsets.push_back({dx, dy, dz});

    double E_pot = 0.0;

    for (int xC = 0; xC < numCells; xC++)
    {
        for (int yC = 0; yC < numCells; yC++)
        {
            for (int zC = 0; zC < numCells; zC++)
            {
                int idx = xC + yC * numCells + zC * numCells * numCells;
                const auto &cCell = cells[idx];

                for (size_t i = 0; i < cCell.size(); i++)
                {
                    for (size_t j = i + 1; j < cCell.size(); j++)
                        E_pot += m_molecules[cCell[i]].potential_energy(m_molecules[cCell[j]], m_boxSize);
                }

                for (auto &off : offsets)
                {
                    int dx = off[0], dy = off[1], dz = off[2];
                    if (dx == 0 && dy == 0 && dz == 0)
                        continue;

                    int nx = (xC + dx + numCells) % numCells;
                    int ny = (yC + dy + numCells) % numCells;
                    int nz = (zC + dz + numCells) % numCells;
                    int nIdx = nx + ny * numCells + nz * numCells * numCells;
                    const auto &nCell = cells[nIdx];

                    for (auto iID : cCell)
                        for (auto jID : nCell)
                            E_pot += m_molecules[iID].potential_energy(m_molecules[jID], m_boxSize);
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
