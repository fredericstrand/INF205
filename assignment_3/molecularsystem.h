
// molecularsystem.h
#ifndef MOLECULARSYSTEM_H
#define MOLECULARSYSTEM_H

#include "molecule.h"
#include <vector>
#include <array>
#pragma once
#include <vector>
#include "molecule.h"

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

    int determineOptimalCellCount() const;
};
