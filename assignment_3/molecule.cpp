#include "molecule.h"
#include <cmath>

Molecule::Molecule(int id, double x, double y, double z, double vx, double vy, double vz)
    : m_id(id), m_coords({x, y, z}), m_vels({vx, vy, vz})
{
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

double Molecule::kinetic_energy(double mass) const
{
    double vx = m_vels[0];
    double vy = m_vels[1];
    double vz = m_vels[2];
    return 0.5 * mass * (vx * vx + vy * vy + vz * vz);
}

double Molecule::potential_energy(const Molecule &other, double boxSize, double epsilon) const
{
    static const double cutoffDistance = 2.5;
    static const double cutoffDistance2 = cutoffDistance * cutoffDistance;

    // Shift potential to zero at r = 2.5:
    static const double u_cut = 4.0 * ((1.0 / std::pow(cutoffDistance, 12)) -
                                       (1.0 / std::pow(cutoffDistance, 6)));

    // Compute distance with the minimum-image convention
    double dx = m_coords[0] - other.m_coords[0];
    double dy = m_coords[1] - other.m_coords[1];
    double dz = m_coords[2] - other.m_coords[2];

    // Apply periodic boundary conditions
    dx -= boxSize * std::round(dx / boxSize);
    dy -= boxSize * std::round(dy / boxSize);
    dz -= boxSize * std::round(dz / boxSize);

    double r2 = dx * dx + dy * dy + dz * dz;

    if (r2 >= cutoffDistance2 || r2 == 0.0)
    {
        return 0.0;
    }

    double inv_r2 = 1.0 / r2;
    double inv_r6 = inv_r2 * inv_r2 * inv_r2;
    double inv_r12 = inv_r6 * inv_r6;

    double u = 4.0 * epsilon * (inv_r12 - inv_r6);
    return u - u_cut;
}