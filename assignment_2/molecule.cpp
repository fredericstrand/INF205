#include "Molecule.h"

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

int Molecule::getID() const {
    return m_id;
}

const std::vector<float>& Molecule::getCoordinates() const {
    return m_coords;
}

const std::vector<float>& Molecule::getVelocities() const {
    return m_vels;
}
