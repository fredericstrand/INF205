#include "molecularsystem.h"

void MolecularSystem::addMolecule(const Molecule &m)
{
    m_mols.push_back(m);
}

double MolecularSystem::totalKinetic() const
{
    double sum = 0.0;
    for (const auto &mol : m_mols)
    {
        sum += mol.kinetic_energy();
    }
    return sum;
}

double MolecularSystem::totalPotential() const
{
    double sum = 0.0;
    for (size_t i = 0; i < m_mols.size(); ++i)
    {
        for (size_t j = i + 1; j < m_mols.size(); ++j)
        {
            sum += m_mols[i].potential_energy(m_mols[j], 1.0, 1.0);
        }
    }
    return sum;
}

const std::vector<Molecule> &MolecularSystem::getMolecules() const
{
    return m_mols;
}
