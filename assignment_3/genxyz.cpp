#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

int genxyz(int box_size, float density, std::string filename)
{
    int N = box_size * box_size * box_size * density;
    float jitter_factor = 0.2;

    double spacing = double(box_size) / std::pow(N, 1.0 / 3.0);

    std::ofstream file(filename);

    for (int i = 0; i < box_size; i++)
    {
        for (int j = 0; j < box_size; j++)
        {
            for (int k = 0; k < box_size; k++)
            {
                double x = (i + 0.5) * spacing;
                double y = (j + 0.5) * spacing;
                double z = (k + 0.5) * spacing;

                x += (2.0 * (double(rand()) / RAND_MAX) - 1.0) * jitter_factor * spacing;
                y += (2.0 * (double(rand()) / RAND_MAX) - 1.0) * jitter_factor * spacing;
                z += (2.0 * (double(rand()) / RAND_MAX) - 1.0) * jitter_factor * spacing;

                x = std::fmod(x + box_size, box_size);
                y = std::fmod(y + box_size, box_size);
                z = std::fmod(z + box_size, box_size);

                file << "C " << x << " " << y << " " << z << std::endl;
            }
        }
    }
    file.close();
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "You need the box_size, density and the filename for output file " << argv[0] << "\n";
        return 1;
    }

    int box_size = std::stoi(argv[1]);
    float density = std::stof(argv[2]);
    std::string filename = argv[3];

    int min_size = 5;

    return genxyz(box_size, density, filename);
}
