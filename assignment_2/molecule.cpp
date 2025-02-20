#include "molecule.h"
#include <cmath>
#include <iostream>

Molecule::Molecule(int id, double x, double y, double z, double vx, double vy, double vz)
    : m_id(id), m_coords({x, y, z}), m_vels({vx, vy, vz})
{
    m_coords[0] = x;
    m_coords[1] = y;
    m_coords[2] = z;

    m_vels[0] = vx;
    m_vels[1] = vy;
    m_vels[2] = vz;
}

// Kinetic energy
double Molecule::kinetic_energy(double mass) const
{
    double vx = m_vels[0];
    double vy = m_vels[1];
    double vz = m_vels[2];
    return 0.5 * mass * (vx * vx + vy * vy + vz * vz);
}

// Potential energy (Lennard-Jones)
double Molecule::potential_energy(const Molecule &other, double epsilon, double sigma) const
{
    static const double cutoffDistance = 2.5;
    static const double u_cut = 4.0 * ((1.0 / std::pow(cutoffDistance, 12)) - (1.0 / std::pow(cutoffDistance, 6)));
    static const double cutoffDistance2 = cutoffDistance * cutoffDistance;

    // Compute inter-molecular distance
    double dx = m_coords[0] - other.m_coords[0];
    double dy = m_coords[1] - other.m_coords[1];
    double dz = m_coords[2] - other.m_coords[2];
    double r2 = dx * dx + dy * dy + dz * dz;

    // If r is greater than cutoff or zero, return zero
    if (r2 >= cutoffDistance2 || r2 == 0.0)
        return 0.0;

    double r = std::sqrt(r2);

    // Instead of std::pow, use multiplications for performance:
    double sr = sigma / r;
    double sr2 = sr * sr;
    double sr3 = sr2 * sr;
    double sr6 = sr3 * sr3;
    double sr12 = sr6 * sr6;

    double u = 4.0 * epsilon * (sr12 - sr6);
    return u - u_cut;
}

int Molecule::getID() const
{
    return m_id;
}

const std::array<double, 3> &Molecule::getCoordinates() const
{
    return m_coords;
}

const std::array<double, 3> &Molecule::getVelocities() const
{
    return m_vels;
}
