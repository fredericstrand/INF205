#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

vector<vector<double>> readXYZ(const string &filename)
{
    vector<vector<double>> data;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Could not open file: " << filename << endl;
        return data;
    }

    int n;
    file >> n;
    string dummy;
    std::getline(file, dummy);

    for (int i = 0; i < n; i++)
    {
        string label;
        double x, y, z;
        file >> label >> x >> y >> z;
        data.push_back({x, y, z});
    }
    return data;
}