#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <string>

using namespace std;

bool Molecule::readXYZ(const string &filename)
{
    // Read the two first lines
    file >> num_atoms;
    file.ignore();
    getline(file, comment);

    // Read the atom data
    string element;
    float x, y, z;
    atoms.clear();
    for (int i = 0; i < num_atoms; i++)
    {
        file >> element >> x >> y >> z;
        atoms.emplace_back(element, x, y, z);
    }
    file.close();
    return true;
}