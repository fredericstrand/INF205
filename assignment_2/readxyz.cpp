#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

vector<vector<float>> readXYZ(const string &filename)
{
    vector<vector<float>> data;
    ifstream file(filename);
    if (!file.is_open())
        return data;
    int n;
    file >> n;
    string num_atoms;
    std::getline(file, num_atoms);
    string dummy
    std::getline(file, dummy)

    for (int = 0; i < n; i++)
    {
        string label;
        float x, y, z;
        file label >> x >> y >> z;
        data.push_back({x, y, z})
    }
    return data;
}