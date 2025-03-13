#ifndef MOLECULE_H
#define MOLECULE_H

#include <array>

class Molecule
{
public:
    Molecule(int id, double x, double y, double z,
             double vx = 0.0, double vy = 0.0, double vz = 0.0);

    int get_ID() const;
    const std::array<double, 3> &get_coordinates() const;
    const std::array<double, 3> &get_velocities() const;

    double kinetic_energy(double mass = 1.0) const;

    // boxSize and epsilon default to 1.0-based LJ units
    double potential_energy(const Molecule &other,
                            double boxSize,
                            double epsilon = 1.0,
                            double sigma = 1.0) const;

private:
    int m_id;
    std::array<double, 3> m_coords;
    std::array<double, 3> m_vels;
};

#endif