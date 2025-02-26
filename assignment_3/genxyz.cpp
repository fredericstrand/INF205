#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

int genxyz(int box_size, float density, std::string filename)
{
    int N = box_size * box_size * box_size * density;
    float jitter_factor = 0.2;
    int min_size = 5;

    double spacing = double(box_size) / std::pow(N, 1.0 / 3.0);

    std::ofstream file(filename);

    file << N << std::endl;
    file << std::endl;

    if (N >= min_size)
    {
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

    std::cerr << "Error: N (" << N << ") is smaller than min_size (" << min_size << "). No file created.\n";
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <box_size> <density> <output_filename>\n";
        return 1;
    }

    int box_size = std::stoi(argv[1]);
    float density = std::stof(argv[2]);
    std::string filename = argv[3];

    return genxyz(box_size, density, filename);
}
