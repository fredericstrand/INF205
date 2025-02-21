#pragma once
#include <vector>
#include "molecule.h"

class MolecularSystem
{
public:
    void addMolecule(const Molecule &m);

    double totalKinetic() const;

    double totalPotential() const;

    // Getter for the internal molecules.
    const std::vector<Molecule> &getMolecules() const;

    ~MolecularSystem();

private:
    std::vector<Molecule> m_mols;
};
