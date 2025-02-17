#include "molecule.h"
#include <cmath>

Molecule::Molecule(int id, double x, double y, double z, double vx, double vy, double vz)
    : m_id(id), m_coords(3), m_vels(3)
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
    double dx = m_coords[0] - other.m_coords[0];
    double dy = m_coords[1] - other.m_coords[1];
    double dz = m_coords[2] - other.m_coords[2];
    double r2 = dx * dx + dy * dy + dz * dz;
    double r = std::sqrt(r2);

    // Checks if r is greater than 2.5 or is 0
    if (r >= 2.5 || r == 0.0)
        return 0.0;

    double sr = sigma / r;
    double sr6 = std::pow(sr, 6);
    double sr12 = std::pow(sr, 12);
    return 4.0 * epsilon * (sr12 - sr6);
}

int Molecule::getID() const
{
    return m_id;
}

const std::vector<double> &Molecule::getCoordinates() const
{
    return m_coords;
}

const std::vector<double> &Molecule::getVelocities() const
{
    return m_vels;
}
