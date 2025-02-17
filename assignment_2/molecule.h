#pragma once
#include <vector>

class Molecule
{
public:
    Molecule(int id, double x, double y, double z, double vx, double vy, double vz);

    // Gettere
    int getID() const;
    const std::vector<double> &getCoordinates() const;
    const std::vector<double> &getVelocities() const;

    double kinetic_energy(double mass = 1) const;
    double potential_energy(const Molecule &other,
                            double epsilon = 1,
                            double sigma = 1) const;

private:
    int m_id;
    std::vector<double> m_coords;
    std::vector<double> m_vels;
};
