#include <tuple>

using namespace std;

class Molecule
{
public:
    // Defining tuples for coordinates and velocities
    int molecule_id = 0;
    tuple<float, float, float> coordinates;
    tuple<float, float, float> velocities;

    // Constructor to initialize coordinates and velocities
    Molecule(float x, float y, float z, float vx, float vy, float vz)
        : coordinates(x, y, z), velocities(vx, vy, vz) {}
};
