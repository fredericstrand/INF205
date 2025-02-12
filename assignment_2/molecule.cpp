#include <tuple>

using namespace std;

class Molecule
{
public:
    // Constructor to initialize coordinates and velocities
    Molecule(float x, float y, float z, float vx, float vy, float vz)
        : coordinates(x, y, z), velocities(vx, vy, vz) {}

private:
    // Defining tuples for coordinates and velocities
    int molecule_id = 0;
    vector<float, float, float> coordinates;
    vector<float, float, float> velocities;
};
