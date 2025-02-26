#pragma once
#include <array>

class Molecule
{
public:
    Molecule(int id, double x, double y, double z,
             double vx, double vy, double vz);

    int getID() const;
    const std::array<double, 3> &getCoordinates() const;
    const std::array<double, 3> &getVelocities() const;

    double kinetic_energy(double mass = 1.0) const;

    double potential_energy(const Molecule &other,
                            double boxSize,
                            double epsilon = 1.0) const;

private:
    int m_id;
    std::array<double, 3> m_coords;
    std::array<double, 3> m_vels;
};
