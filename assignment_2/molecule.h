#pragma once
#include <array>

class Molecule
{
public:
    Molecule(int id, double x, double y, double z, double vx, double vy, double vz);

    // Gettere
    int getID() const;
    const std::array<double, 3> &getCoordinates() const;
    const std::array<double, 3> &getVelocities() const;

    double kinetic_energy(double mass = 1) const;
    double potential_energy(const Molecule &other,
                            double epsilon = 1,
                            double sigma = 1) const;

private:
    int m_id;
    std::array<double, 3> m_coords;
    std::array<double, 3> m_vels;
};
