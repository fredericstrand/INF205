#pragma once
#include <vector>

class Molecule
{
public:
    Molecule(int id, float x, float y, float z, float vx, float vy, float vz);

    int getID() const;
    const std::vector<float> &getCoordinates() const;
    const std::vector<float> &getVelocities() const;

    float kinetic_energy(float mass = 1) const;
    float potential_energy(const Molecule &other, float epsilon = 1, float sigma = 1) const;

private:
    int m_id;
    std::vector<float> m_coords;
    std::vector<float> m_vels;
};
