#include "molecule.h"
#include <cmath> // for std::sqrt, std::pow

Molecule::Molecule(int id, float x, float y, float z, float vx, float vy, float vz)
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
float Molecule::kinetic_energy(float mass) const
{
    float vx = m_vels[0];
    float vy = m_vels[1];
    float vz = m_vels[2];
    return 0.5f * mass * (vx * vx + vy * vy + vz * vz);
}

// Potential energy (e.g., Lennard-Jones)
float Molecule::potential_energy(const Molecule &other, float epsilon, float sigma) const
{
    float dx = m_coords[0] - other.m_coords[0];
    float dy = m_coords[1] - other.m_coords[1];
    float dz = m_coords[2] - other.m_coords[2];
    float r2 = dx * dx + dy * dy + dz * dz;
    float r = std::sqrt(r2);
    if (r == 0.0f)
        return 0.0f;

    float sr = sigma / r;
    float sr6 = std::pow(sr, 6);
    float sr12 = std::pow(sr, 12);
    return 4.0f * epsilon * (sr12 - sr6);
}

int Molecule::getID() const
{
    return m_id;
}

const std::vector<float> &Molecule::getCoordinates() const
{
    return m_coords;
}

const std::vector<float> &Molecule::getVelocities() const
{
    return m_vels;
}
