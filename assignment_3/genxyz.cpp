#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <boxSize> <density> <out.xyz>\n";
        return 1;
    }
    double boxSize = std::atof(argv[1]);
    if (boxSize < 5.0)
    {
        std::cerr << "Box size must be >=5.\n";
        return 1;
    }
    double rho = std::atof(argv[2]);
    std::string outFile = argv[3];

    double volume = boxSize * boxSize * boxSize;
    int N = (int)std::round(rho * volume);
    if (N < 1)
    {
        std::cerr << "Too few atoms.\n";
        return 1;
    }

    std::srand((unsigned)std::time(nullptr));
    int nCbrt = (int)std::cbrt(N);
    while (nCbrt * nCbrt * nCbrt < N)
        nCbrt++;
    double spacing = boxSize / double(nCbrt);
    double jitter = 0.2 * spacing;

    std::ofstream ofs(outFile);
    ofs << N << "\n\n";
    int count = 0;
    for (int ix = 0; ix < nCbrt && count < N; ix++)
    {
        for (int iy = 0; iy < nCbrt && count < N; iy++)
        {
            for (int iz = 0; iz < nCbrt && count < N; iz++)
            {
                double x = (ix + 0.5) * spacing;
                double y = (iy + 0.5) * spacing;
                double z = (iz + 0.5) * spacing;
                x += (2.0 * rand() / RAND_MAX - 1.0) * jitter;
                y += (2.0 * rand() / RAND_MAX - 1.0) * jitter;
                z += (2.0 * rand() / RAND_MAX - 1.0) * jitter;
                if (x < 0)
                    x += boxSize;
                else if (x >= boxSize)
                    x -= boxSize;
                if (y < 0)
                    y += boxSize;
                else if (y >= boxSize)
                    y -= boxSize;
                if (z < 0)
                    z += boxSize;
                else if (z >= boxSize)
                    z -= boxSize;
                ofs << "C " << x << " " << y << " " << z << "\n";
                count++;
            }
        }
      
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
    ofs.close();
    return 0;
}
