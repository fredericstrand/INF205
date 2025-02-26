#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>

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

    if (file.fail())
    {
        std::cerr << "Error: Could not read the number of molecules in " << filename << std::endl;
        return data;
    }

    std::string dummy;
    std::getline(file, dummy);

    std::cout << "Expected number of molecules: " << n << std::endl;

    for (int i = 0; i < n; i++)
    {
        std::string label;
        double x, y, z;

        if (!(file >> label >> x >> y >> z))
        {
            std::cerr << "Error: Could not read line " << i + 1 << " in " << filename << std::endl;
            break;
        }

        data.push_back({x, y, z});
    }

    std::cout << "Successfully read " << data.size() << " molecules from " << filename << std::endl;
    return data;
}
