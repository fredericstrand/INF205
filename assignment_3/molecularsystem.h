#pragma once
#include <vector>
#include "molecule.h"

class MolecularSystem
{
public:
    explicit MolecularSystem(double boxSize);

    void addMolecule(const Molecule &molecule);

    double totalKineticEnergy() const;
    double totalPotentialEnergy() const;
    double totalPotentialEnergyLinkedCells() const;
    double totalEnergy() const;

    const std::vector<Molecule> &getMolecules() const;

private:
    double m_boxSize;
    std::vector<Molecule> m_molecules;

    // Helper method to determine the optimal cell count
    int determineOptimalCellCount() const;
};