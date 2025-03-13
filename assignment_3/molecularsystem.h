// molecularsystem.h
#ifndef MOLECULARSYSTEM_H
#define MOLECULARSYSTEM_H

#include "molecule.h"
#include <vector>
#include <array>

class MolecularSystem
{
public:
    MolecularSystem(double a);
    void add_molecule(const Molecule &mol);
    double total_kinetic_energy() const;
    double total_potential_energy() const;
    double total_potential_energy_LinkedCells() const;
    double total_energy() const;
    const std::vector<Molecule> &get_molecules() const;

private:
    double box_size;
    std::vector<Molecule> molecules;
    int determineOptimalCellCount() const;
};

#endif
