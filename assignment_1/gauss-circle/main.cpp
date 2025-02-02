#include <iostream>
#include <cstdlib>
#include "is-gauss.h"

int main(int argc, char **argv)
{
    // Check if the radius is valid
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <radius>\n";
        return 1;
    }

    // Parse the radius from the command-line argument
    int r = std::atoi(argv[1]);
    if (r <= 0)
    {
        std::cerr << "Error: Radius must be a positive.\n";
    }

    int count = num_points(r);

    std::cout << "Number of points for radius " << r << " -> " << count << std::endl;
}
