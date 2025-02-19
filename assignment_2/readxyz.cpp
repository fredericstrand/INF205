#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>

std::vector<std::array<double, 3>> readXYZ(const std::string &filename)
{
    std::vector<std::array<double, 3>> data;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Could not open file: " << filename << std::endl;
        return data;
    }

    int n;
    file >> n;
    std::string dummy;
    std::getline(file, dummy);

    for (int i = 0; i < n; i++)
    {
        std::string label;
        double x, y, z;
        file >> label >> x >> y >> z;
        data.push_back({x, y, z});
    }
    return data;
}