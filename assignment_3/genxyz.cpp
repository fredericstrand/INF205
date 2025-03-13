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
    }
    ofs.close();
    return 0;
}
