#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>

int genxyz(int box_size, float density, const std::string &filename)
{
    // Calculate desired total number of atoms
    int N = static_cast<int>(std::round(box_size * box_size * box_size * density));

    // Jitter factor / spacing factor
    float jitter_factor = 0.2f;
    int min_size = 5; // if fewer than min_size, bail out

    // If N < min_size, we won't create the file
    if (N < min_size)
    {
        std::cerr << "Error: N (" << N
                  << ") is smaller than min_size (" << min_size
                  << "). No file created.\n";
        return 1;
    }

    // Figure out an integer nCbrt so nCbrt^3 >= N
    int nCbrt = static_cast<int>(std::cbrt(N));
    while (nCbrt * nCbrt * nCbrt < N)
    {
        nCbrt++;
    }

    // Calculate spacing so that nCbrt points span 'box_size'
    double spacing = static_cast<double>(box_size) / nCbrt;

    // Open file
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    // Write header: number of atoms + blank line
    file << N << std::endl;
    file << std::endl;

    // Place exactly N atoms on this smaller 3D grid
    int count = 0;
    for (int i = 0; i < nCbrt && count < N; i++)
    {
        for (int j = 0; j < nCbrt && count < N; j++)
        {
            for (int k = 0; k < nCbrt && count < N; k++)
            {
                // center of each cell
                double x = (i + 0.5) * spacing;
                double y = (j + 0.5) * spacing;
                double z = (k + 0.5) * spacing;

                // add random jitter (± jitter_factor * spacing)
                x += (2.0 * (double(rand()) / RAND_MAX) - 1.0) * jitter_factor * spacing;
                y += (2.0 * (double(rand()) / RAND_MAX) - 1.0) * jitter_factor * spacing;
                z += (2.0 * (double(rand()) / RAND_MAX) - 1.0) * jitter_factor * spacing;

                // wrap using periodic boundary conditions
                x = std::fmod(x + box_size, box_size);
                y = std::fmod(y + box_size, box_size);
                z = std::fmod(z + box_size, box_size);

                // output: "C x y z"
                file << "C " << x << " " << y << " " << z << std::endl;

                ++count; // one more atom placed
            }
        }
    }

    file.close();
    return 0;
}

int main(int argc, char *argv[])
{
    // seed random for jitter
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <box_size> <density> <output_filename>\n";
        return 1;
    }

    int box_size = std::stoi(argv[1]);
    float density = std::stof(argv[2]);
    std::string filename = argv[3];

    return genxyz(box_size, density, filename);
}
